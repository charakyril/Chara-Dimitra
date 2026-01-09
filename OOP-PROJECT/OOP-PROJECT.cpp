#include <iostream>        
#include <string>          
#include <cctype>          
#include <cmath>           
#include "types.h"         
#include "world.h"         
#include "sensor.h"        
#include "navigation.h"
#include "fusion.h"
#include "sensor_reading.h"
#include "world_manager.h"
#include <random>                        
#include <chrono>          

using namespace std;

/// Global simulation tick counter - tracks the number of time steps in simulation
/// This variable can be used for time-based behaviors and event scheduling
unsigned int ticks = 0;

/// Global GPS position variable - stores the current navigation target
/// This is used by the navigation system to determine movement direction
Position gps_pos;

class SelfDrivingCar
{
    protected:
        // Current direction of travel for the vehicle (unit vector: -1, 0, or 1 for x/y)
        Direction car_direction;
        // Current speed state - affects movement per tick
        // speed = "STOPPED", "HALF_SPEED", "FULL_SPEED"
        string speed;
        // Current position on the world grid (x, y coordinates)
        Position position{0,0};
        Lidar ld_sensor;      ///< LIDAR sensor - 360-degree object detection
        Radar rd_sensor;      ///< RADAR sensor - front-facing, moving object detection
        Camera cam_sensor;    ///< Camera sensor - visual recognition (signs, lights)
        // Navigation system for GPS-based path planning and decision making
        NavigationSystem nav_system;
    public:
        //Constructor
        SelfDrivingCar(Direction dir, const string& sp, const Position& pos, Lidar lidar, Radar radar, Camera camera, NavigationSystem nav)
        : car_direction(dir), speed(sp), position(pos), ld_sensor(lidar), rd_sensor(radar), cam_sensor(camera), nav_system(nav)
        {
            // Convert direction to string for display
            string dirStr;
            if (dir.x == 1) dirStr = "EAST";
            else if (dir.x == -1) dirStr = "WEST";
            else if (dir.y == 1) dirStr = "NORTH";
            else dirStr = "SOUTH";
            
            cout << "Initialized at ("<< position.x <<","<< position.y <<")" 
            << " facing " << dirStr << " - No driver's license required!" << endl;
            // initialize camera's notion of the car position
            cam_sensor.position = position;
        }
        //Destructor
        ~SelfDrivingCar() 
        {
            cout << "Our journey is complete! Goodbye :)" << endl;
        }

        //Getter function
        const Position& getPosition() const { return position; }

        // Collect raw sensor readings from the world
        vector<SensorReading> collectSensorData(const World& world, mt19937_64& rng) 
        {
            vector<SensorReading> out;
            uniform_real_distribution<float> noise(-0.05f, 0.05f);

            for (auto p : world.objects) 
            {
                //Finds distance
                Position objPos = p->getPosition();
                int dist = abs(objPos.x - position.x) + abs(objPos.y - position.y);

                // LIDAR (360-degree)
                if (dist <= static_cast<int>(ld_sensor.RANGE)) 
                {
                    //Enters data for lidar ( values that lidar returns )
                    SensorReading r;
                    //Enters ID, TYPE, POSITION and DISTANCE
                    r.objectId = p->getID(); r.type = p->getType(); r.position = objPos; r.distance = static_cast<float>(dist);
                    //Then, finds confidence of lidar
                    float base = static_cast<float>(ld_sensor.ACCURACY) / 100.0f;
                    r.confidence = max(0.0f, min(1.0f, base * (1.0f - (float)dist / (ld_sensor.RANGE + 1)) + noise(rng)));
                    out.push_back(r);
                }

                // RADAR (straight ahead, only moving objects)
                if (dist <= static_cast<int>(rd_sensor.RANGE)) 
                {
                    // check moving
                    if (dynamic_cast<MovingObjects*>(p) != nullptr) 
                    {
                        //Enters data for radar ( values that radar returns )
                        SensorReading r;
                        //ID, TYPE, POSITION, DISTANCE
                        r.objectId = p->getID(); r.type = p->getType(); r.position = objPos; r.distance = static_cast<float>(dist);
                        //Finds confidence of radar
                        float base = static_cast<float>(rd_sensor.ACCURACY) / 100.0f;
                        r.confidence = max(0.0f, min(1.0f, base * (1.0f - (float)dist / (rd_sensor.RANGE + 1)) + noise(rng)));
                        out.push_back(r);
                    }
                }

                // CAMERA (front 7x7, simplified as within range)
                if (dist <= static_cast<int>(cam_sensor.RANGE)) 
                {
                    //Camera returns all possible values
                    SensorReading r;
                    //ID, TYPE, POSITION, DISTANCE
                    r.objectId = p->getID(); r.type = p->getType(); r.position = objPos; r.distance = static_cast<float>(dist);
                    //Finds confidence of camera
                    float base = static_cast<float>(cam_sensor.ACCURACY) / 100.0f;
                    r.confidence = max(0.0f, min(1.0f, base * (1.0f - (float)dist / (cam_sensor.RANGE + 1)) + noise(rng)));
                    //Also values for traffic signs and traffic lights, if provided
                    //If traffic light, try to get its colour
                    if (auto tl = dynamic_cast<TRAFFIC_LIGHTS*>(p)) r.lightColour = tl->getColour();
                    if (dynamic_cast<TRAFFIC_SIGNS*>(p)) r.signText = "STOP"; // basic
                    out.push_back(r);
                }
            }
            //Return the SensorReading (out) with all the values
            return out;
        }

        //Sync with navigation system by fusing data
        vector<SensorReading> syncNavigationSystem(NavigationSystem& nav, const vector<SensorReading>& raw) 
        {
            auto fused = nav.fuseSensorData(raw);
            return fused;
        }

        // execute movement: apply decision and update position
        // returns true if moved outside world (end condition)
        bool executeMovement(const Direction& dir, const string& action, const World& world) 
        {
            //Adjust speed
            //Speed goes from STOPPED to HALF_SPEED 
            //and from HALF_SPEED to FULL_SPEED
            if (action == "ACCELERATE") 
            {
                if (speed == "STOPPED") speed = "HALF_SPEED";
                else if (speed == "HALF_SPEED") speed = "FULL_SPEED";
            } 
            //Speed goes from FULL_SPEED to HALF_SPEED
            //and from HALF_SPEED to STOPPED
            else if (action == "DECELERATE") 
            {
                if (speed == "FULL_SPEED") speed = "HALF_SPEED";
                else if (speed == "HALF_SPEED") speed = "STOPPED";
            }
            //Adjust number of steps, according to speed
            int steps = 0;
            if (speed == "FULL_SPEED") steps = 2; //FULL_SPEED
            else if (speed == "HALF_SPEED") steps = 1; //HALF_SPEED
            else steps = 0; // STOPPED

            //If SelfDrivingCar is not STOPPED
            if (steps > 0) 
            {
               // Determine actual number of steps (clamp to avoid overshooting target)
               int maxSteps = steps;
               auto tgtOpt = nav_system.getCurrentTarget();
               
               //If navigation target exists and movement direction exists
               if (tgtOpt.has_value() && (dir.x != 0 || dir.y != 0)) 
               {
                   Position tgt = *tgtOpt;
                   // Calculate steps needed in each direction to reach target exactly
                   if (dir.x != 0) 
                   {
                       int stepsToTargetX = abs(tgt.x - position.x);
                       //If needed, limits the steps so car doesn't go further than target
                       if (steps > stepsToTargetX) maxSteps = stepsToTargetX;
                   }
                   if (dir.y != 0) 
                   {
                       int stepsToTargetY = abs(tgt.y - position.y);
                       if (steps > stepsToTargetY) maxSteps = stepsToTargetY;
                   }
               }
               //For every step
               for (int i = 0; i < maxSteps; ++i) 
               {
                   //Calculates new position
                   int nx = position.x + static_cast<int>(dir.x);
                   int ny = position.y + static_cast<int>(dir.y);
                   //Checks if went out of bounds
                   if (nx < 0 || ny < 0 || static_cast<unsigned int>(nx) >= world.getDimX() || static_cast<unsigned int>(ny) >= world.getDimY()) 
                   {
                       //Returns true if attempted to go out of bounds
                       return true;
                   }
                   //Else, updates position
                   position.x = nx; position.y = ny;
               }

               //Update camera position
               cam_sensor.position = position;
           }

           //Inform nav system about arrival check
           nav_system.checkArrival(position);
           //Returns false if movement was inside of world bounds
           return false;

        }

        //Getter function
        NavigationSystem& getNavigation() { return nav_system; }
        string getSpeed() const { return speed; }

        //Describe function for visualization
        void describe() const 
        {
            cout << "SelfDrivingCar at (" << position.x << "," << position.y << ") speed=" << speed << "\n";
        }
};

//---------MAIN--------//

//GPS targets as arguments from user
int main(int argc, char* argv[]) 
{

    // Defaults
    unsigned long long seed = static_cast<unsigned long long>(
    chrono::system_clock::now().time_since_epoch().count());
    unsigned int dimX = 40;
    unsigned int dimY = 40;
    unsigned int numMovingCars = 3;
    unsigned int numMovingBikes = 4;
    unsigned int numParkedCars = 5;
    unsigned int numStopSigns = 2;
    unsigned int numTrafficLights = 2;
    unsigned int simulationTicks = 100;
    unsigned int minConfidenceThreshold = 40; // percent
    //Vector for gps targets
    vector<Position> gpsTargets;

    //Prints if user types --help
    auto printHelp = []() 
    {
        cout << "Self-Driving Car Simulation" << endl;
        cout << "Usage:" << endl;
        cout << "  --seed <n>              " << "Random seed (default: current time)" << endl;
        cout << "  --dimX <n>              " << "World width (default: 40)" << endl;
        cout << "  --dimY <n>              " << "World height (default: 40)" << endl;
        cout << "  --numMovingCars <n>     " << "Number of moving cars (default: 3)" << endl;
        cout << "  --numMovingBikes <n>    " << "Number of moving bikes (default: 4)" << endl;
        cout << "  --numParkedCars <n>     " << "Number of parked cars (default: 5)" << endl;
        cout << "  --numStopSigns <n>      " << "Number of stop signs (default: 2)" << endl;
        cout << "  --numTrafficLights <n>  " << "Number of traffic lights (default: 2)" << endl;
        cout << "  --simulationTicks <n>   " << "Maximum simulation ticks (default: 100)" << endl;
        cout << "  --minConfidenceThreshold <n> " << "Min confidence cutoff (default: 40)" << endl;
        cout << "  --gps <x1> <y1> [x2 y2 ...] " << "GPS target coordinates (required)" << endl;
        cout << "  --help                  " << "Show this help message" << endl << endl;
        cout << "Example usage:" << endl;
        cout << "  ./oopproj_2025 --seed 12 --dimY 50 --gps 10 20 32 15" << endl;
    };

    //Pass arguments
    for (int i = 1; i < argc; ++i) 
    {
        //Convert to string for comparison
        string a = argv[i];

        //Check all possible things user could have typed
        if (a == "--help") 
        {
            printHelp();
            return 0;
        } 
        else if (a == "--seed") 
        {
            //Check for proper seed income
            if (i + 1 >= argc) { cerr << "--seed requires a value\n"; return 1; }
            try { seed = stoull(argv[++i]); } catch (...) { cerr << "Invalid --seed\n"; return 1; }
        } 
        else if (a == "--dimX") 
        {
            //Check for correct X dimention
            if (i + 1 >= argc) { cerr << "--dimX requires a value\n"; return 1; }
            try { dimX = stoul(argv[++i]); } catch (...) { cerr << "Invalid --dimX\n"; return 1; }
        } 
        else if (a == "--dimY") 
        {
            //Check for correct Y dimention
            if (i + 1 >= argc) { cerr << "--dimY requires a value\n"; return 1; }
            try { dimY = stoul(argv[++i]); } catch (...) { cerr << "Invalid --dimY\n"; return 1; }
        }
        //Checks for args to initialize objects in grid world
        //Moving cars 
        else if (a == "--numMovingCars") 
        {
            if (i + 1 >= argc) { cerr << "--numMovingCars requires a value\n"; return 1; }
            try { numMovingCars = stoul(argv[++i]); } catch (...) { cerr << "Invalid --numMovingCars\n"; return 1; }
        } 
        //Moving bikes
        else if (a == "--numMovingBikes") 
        {
            if (i + 1 >= argc) { cerr << "--numMovingBikes requires a value\n"; return 1; }
            try { numMovingBikes = stoul(argv[++i]); } catch (...) { cerr << "Invalid --numMovingBikes\n"; return 1; }
        } 
        //Parked cars
        else if (a == "--numParkedCars") 
        {
            if (i + 1 >= argc) { cerr << "--numParkedCars requires a value\n"; return 1; }
            try { numParkedCars = stoul(argv[++i]); } catch (...) { cerr << "Invalid --numParkedCars\n"; return 1; }
        } 
        //STOP signs
        else if (a == "--numStopSigns") 
        {
            if (i + 1 >= argc) { cerr << "--numStopSigns requires a value\n"; return 1; }
            try { numStopSigns = stoul(argv[++i]); } catch (...) { cerr << "Invalid --numStopSigns\n"; return 1; }
        } 
        //Traffic lights
        else if (a == "--numTrafficLights") 
        {
            if (i + 1 >= argc) { cerr << "--numTrafficLights requires a value\n"; return 1; }
            try { numTrafficLights = stoul(argv[++i]); } catch (...) { cerr << "Invalid --numTrafficLights\n"; return 1; }
        } 
        //Check if user entered number of ticks for simulation
        else if (a == "--simulationTicks") 
        {
            if (i + 1 >= argc) { cerr << "--simulationTicks requires a value\n"; return 1; }
            try { simulationTicks = stoul(argv[++i]); } catch (...) { cerr << "Invalid --simulationTicks\n"; return 1; }
        }
        //Check if user entered under boundary for sensor's confidence 
        else if (a == "--minConfidenceThreshold") 
        {
            if (i + 1 >= argc) { cerr << "--minConfidenceThreshold requires a value\n"; return 1; }
            try { minConfidenceThreshold = stoul(argv[++i]); } catch (...) { cerr << "Invalid --minConfidenceThreshold\n"; return 1; }
        } 
        //Check for GPS TARGETS
        else if (a == "--gps") 
        {
            // collect trailing tokens until next --flag or end
            int j = i + 1;
            vector<string> tokens;
            while (j < argc && string(argv[j]).rfind("--", 0) != 0) 
            {
                //Stores values in vector
                tokens.push_back(argv[j++]);
            }
            //If no coordinates, error
            if (tokens.empty()) { cerr << "--gps requires at least one pair of coordinates\n"; return 1; }
            //If aneven x,y coordinates, error
            if (tokens.size() % 2 != 0) { cerr << "--gps requires x y pairs (even number of values)\n"; return 1; }
            //For every x,y pair of coordinates
            for (size_t k = 0; k < tokens.size(); k += 2) 
            {
                try 
                {
                    //Convert string to integer and store in gpsTargets vector
                    int x = stoi(tokens[k]);
                    int y = stoi(tokens[k + 1]);
                    gpsTargets.push_back(Position{x, y});
                } catch (...) { cerr << "Invalid GPS coordinates\n"; return 1; }
            }
            i = j - 1; //Advance main loop
        } 
        //If unknown input
        else 
        {
            //Show error and print help for user
            cerr << "Unknown argument: " << a << "\n";
            printHelp();
            return 1;
        }
    }
    //If no gps targets, error (need to have at least one pair of gps targets)
    if (gpsTargets.empty()) 
    {
        cerr << "Error: at least one GPS target must be provided with --gps\n";
        printHelp();
        return 1;
    }

    // Validate GPS targets are within world bounds
    for (const auto& target : gpsTargets) 
    {
        if (target.x < 0 || target.x >= static_cast<int>(dimX) || target.y < 0 || target.y >= static_cast<int>(dimY)) 
        {
            cerr << "Error: GPS target (" << target.x << "," << target.y << ") is out of bounds (world is " << dimX << "x" << dimY << ")\n";
            printHelp();
            return 1;
        }
    }

    // Seed RNG (example)
    mt19937_64 rng(seed);

    // Echo parsed configuration
    cout << "Configuration:" << endl;
    cout << "  seed: " << seed << endl;
    cout << "  dimX x dimY: " << dimX << " x " << dimY << endl;
    cout << "  movingCars: " << numMovingCars << ", movingBikes: " << numMovingBikes << endl;
    cout << "  parkedCars: " << numParkedCars << ", stopSigns: " << numStopSigns << ", trafficLights: " << numTrafficLights << endl;
    cout << "  simulationTicks: " << simulationTicks << endl;
    cout << "  minConfidenceThreshold: " << minConfidenceThreshold << endl;
    cout << "  GPS targets (" << gpsTargets.size() << "):";
    for (const auto& p : gpsTargets) cout << " (" << p.x << "," << p.y << ")";
    cout << endl;

    //Build world and place objects
    World world(dimX, dimY);
    
    //Pick a random position for the self-driving car using the seed
    uniform_int_distribution<int> xdist(0, dimX-1);
    uniform_int_distribution<int> ydist(0, dimY-1);
    Position carPos;
    carPos.x = xdist(rng);
    carPos.y = ydist(rng);

    //Pick a random direction for the self-driving car
    vector<Direction> carDirs = {{1,0},{-1,0},{0,1},{0,-1}};
    uniform_int_distribution<int> carDirDist(0, 3);
    Direction carDir = carDirs[carDirDist(rng)];
    
    //Convert direction to string for display
    string dirStr;
    if (carDir.x == 1) dirStr = "EAST";
    else if (carDir.x == -1) dirStr = "WEST";
    else if (carDir.y == 1) dirStr = "NORTH";
    else dirStr = "SOUTH";
    
    
    // Create sensors and navigation
    Lidar lidar("LIDAR", 9, 9, 87, 0, carPos, 0.99f);
    Radar radar("RADAR", 12, 12, 95, 0, carPos, Direction{0,1}, 0.99f);
    Camera camera("CAM", 7, 7, 95, 0, carPos, "N/A", 0.95f, "STOPPED", Direction{0,1}, "N/A", "GREEN");

    //Pass under boundary for sensor's confidence
    NavigationSystem nav(minConfidenceThreshold);
    //Pass gps Targets to navigation system
    nav.setTargets(gpsTargets);

    //Create SelfDrivingCar
    SelfDrivingCar car(carDir, "STOPPED", carPos, lidar, radar, camera, nav);

    //Populate world with parked cars, signs, lights, moving vehicles
    for (unsigned int i = 0; i < numParkedCars; ++i) 
    {
        //Create parked cars
        Position p;
        do { p.x = xdist(rng); p.y = ydist(rng); } while (!world.isCellFree(p.x, p.y));
        world.addObject(new STAT_VEH('P', "PARKED", p.x, p.y, "PARKED"));
    }
    for (unsigned int i = 0; i < numStopSigns; ++i) 
    {
        //Create stor signs
        Position p;
        do { p.x = xdist(rng); p.y = ydist(rng); } while (!world.isCellFree(p.x, p.y));
        world.addObject(new TRAFFIC_SIGNS('S', "SIGN", p.x, p.y, "SIGN", "STOP"));
    }
    for (unsigned int i = 0; i < numTrafficLights; ++i) 
    {
        //Create traffic lights
        Position p;
        do { p.x = xdist(rng); p.y = ydist(rng); } while (!world.isCellFree(p.x, p.y));
        world.addObject(new TRAFFIC_LIGHTS('L', "LIGHT", p.x, p.y, "LIGHT", "RED"));
    }
    //Moving cars
    //Make vector for direction
    vector<Direction> dirs = {{1,0},{-1,0},{0,1},{0,-1}};
    uniform_int_distribution<int> dirDist(0, static_cast<int>(dirs.size())-1);
    for (unsigned int i = 0; i < numMovingCars; ++i) 
    {
        Position p;
        //Place cars in grid wolrd and adjust direction
        do { p.x = xdist(rng); p.y = ydist(rng); } while (!world.isCellFree(p.x, p.y));
        Direction d = dirs[dirDist(rng)];
        world.addObject(new CARS('C', "CAR", p.x, p.y, "HALF_SPEED", d, "CAR"));
    }
    for (unsigned int i = 0; i < numMovingBikes; ++i) 
    {
        Position p;
        //Place bikes in grid world and adjust direction
        do { p.x = xdist(rng); p.y = ydist(rng); } while (!world.isCellFree(p.x, p.y));
        Direction d = dirs[dirDist(rng)];
        world.addObject(new BIKES('B', "BIKE", p.x, p.y, "HALF_SPEED", d, "BIKE"));
    }

    //Print initial world state
    cout << "\n=== Initial World State ===" << endl;
    //Full visualization
    world.printFull('@', &carPos);
    
    //Simulation loop
    bool outOfBounds = false;
    Direction prevDir = Direction{0, 1}; // default NORTH
    //For every tick and without self driving car getting out of bounds
    for (unsigned int t = 1; t <= simulationTicks && !outOfBounds; ++t) 
    {
        // Check arrival BEFORE decision making so new target is used for next decision
        nav.checkArrivalBeforeTick(car.getPosition());
        
        // Get current target for display
        auto currentTarget = nav.getCurrentTarget();
        string targetStr = "";
        if (currentTarget.has_value()) 
        {
            targetStr = " target=(" + to_string(currentTarget->x) + "," + to_string(currentTarget->y) + ")";
        }
        
        // Update world objects first
        world.updateTick();

        // Collect sensor data
        auto raw = car.collectSensorData(world, rng);
        
        // Fuse and feed to navigation
        auto fused = car.syncNavigationSystem(nav, raw);
        nav.receiveFusedReadings(fused);

        // Make decision using FRESH fused data (not stale data from last tick)
        auto decision = nav.makeDecisionWithFused(car.getPosition(), fused);
        Direction moveDir = decision.first;
        string action = decision.second;

        // Determine actual direction for display (use prevDir if no move)
        Direction displayDir = moveDir;
        if (displayDir.x == 0 && displayDir.y == 0) 
        {
            displayDir = prevDir;
        }
        
        // Print direction string
        string dirStr;
        if (displayDir.x == 1) dirStr = "EAST";
        else if (displayDir.x == -1) dirStr = "WEST";
        else if (displayDir.y == 1) dirStr = "NORTH";
        else if (displayDir.y == -1) dirStr = "SOUTH";
        else dirStr = "SOUTH"; // Default
        
        // Print "Turning from X to Y" if direction changed
        string prevDirStr;
        if (prevDir.x == 1) prevDirStr = "EAST";
        else if (prevDir.x == -1) prevDirStr = "WEST";
        else if (prevDir.y == 1) prevDirStr = "NORTH";
        else if (prevDir.y == -1) prevDirStr = "SOUTH";
        else prevDirStr = "SOUTH";
        
        if (dirStr != prevDirStr && t > 1) 
        {
            cout << "Turning from " << prevDirStr << " to " << dirStr << "\n";
        }
        
        // Get actual speed from car (after executeMovement has updated it)
        string speed = car.getSpeed();
        
        // Print car state (position BEFORE movement for this tick)
        cout << "SelfDrivingCar at (" << car.getPosition().x << "," << car.getPosition().y << ")" << targetStr;
        if (t >= 2) 
        {
            cout << " speed=" << speed << " direction=" << dirStr;
        }
        cout << "\n";
        
        // Update direction for next iteration
        if (moveDir.x != 0 || moveDir.y != 0) 
        {
            prevDir = moveDir;
        }
        
        // Print tick number
        if (t == 1) 
        {
            cout << "  Tick: " << t << "\n";
        } 
        else 
        {
            cout << "Tick: " << t << "\n";
        }

        // Execute movement (updates position for next tick)
        outOfBounds = car.executeMovement(moveDir, action, world);

        // Stop simulation if all targets have been reached
        if (!nav.getCurrentTarget().has_value()) 
        {
            cout << "\n*** All targets reached! Stopping simulation. ***" << endl;
            cout << "\n=== Final World State ===" << endl;
            world.printFull('@', &car.getPosition());
            return 0;
        }

        // Print 7x7 world around the car
        cout << "\nWorld around car (7x7):\n";
        world.printAround(car.getPosition(), 3, '@');
        cout << "\n";
    }

    //If self driving car out of bounds, end simulation
    if (outOfBounds) cout << "Self-driving car attempted to leave the world. Simulation ended.\n";

    //Prints full visualization
    cout << "\n=== Final World State ===" << endl;
    world.printFull('@', &car.getPosition());

    return 0;
}