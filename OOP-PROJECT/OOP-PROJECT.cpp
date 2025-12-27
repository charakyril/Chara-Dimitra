#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
#include "types.h"
#include "navigation.h"
#include "world.h"
#include "sensor.h"
#include <random>

using namespace std;

//Global variable ticks//
unsigned int ticks = 0;

//Global variable gps_pos//
Position gps_pos;

// Shared types (Position, Direction) are defined in types.h


//----CLASS FOR SelfDrivingCar----//
class SelfDrivingCar
{
    protected:
        //Car direction
        Direction car_direction;
        //Car position
        Position car_position;
        //SPEED = STOPPED, HALF_SPEED, FULL_SPEED
        string speed;
        //ALL SENSORS
        Lidar ld_sensor;
        Radar rd_sensor;
        Camera cam_sensor;
        //class navigation system
        NavigationSystem nav_system;
    public:
        //Constructor
        SelfDrivingCar(Direction dir, Position car_pos, const string& sp, Lidar lidar, Radar radar, Camera camera, NavigationSystem nav)
        : car_direction(dir), car_position(car_pos), speed(sp), ld_sensor(lidar), rd_sensor(radar), cam_sensor(camera), nav_system(nav)
        {
            cout << "I made the self driving car\n";
        }
        //Destructor
        ~SelfDrivingCar() 
        {
            cout << "No self driving car\n";
        }
        //Accelerate
        //Accelerates from stopped to half speed and from half speed to full speed
        void Accelerate() const
        {
            if(speed == "STOPPED")
                speed == "HALF_SPEED";
            else if(speed == "HALF_SPEED")
                speed == "FULL_SPEED";
        }
        //Decelerate
        //3 periptwseis pou to car kobei taxythta
        void Decelerate() const
        {
            //An fanari entos 3 thesewn
            //Εδώ δεν χρειάζεται γιατί μόνο ο camera βλέπει χρώμα φαναριών 
            if((cam_sensor.LightColour == "RED" || cam_sensor.LightColour == "YELLOW") && 
            (cam_sensor.distance() <= 3)) //distance of car and object
            {
                if(speed == "FULL_SPEED")
                    speed == "HALF_SPEED";
                else if(speed == "HALF_SPEED")
                    speed == "STOPPED";
            }
            //An stoxos GPS entos 5 thesewn
            //isos xreiazetai συγχώνευση μετρήσεων από τους αισθητήρες
            if(cam_sensor.GPS_distance(gps_pos) <= 5) //distance of car and gps target
            {
                if(speed == "FULL_SPEED")
                    speed == "HALF_SPEED";
                else if(speed == "HALF_SPEED")
                    speed == "STOPPED";
            }
            //An kinoumeno antikeimeno entos 2 thesewn
            //isos xreiazetai συγχώνευση μετρήσεων από τους αισθητήρες
            if(cam_sensor.distance() <= 2) //distance of car and object 
            {
                if(speed == "FULL_SPEED")
                    speed == "HALF_SPEED";
                else if(speed == "HALF_SPEED")
                    speed == "STOPPED";
            }
        }
        //Describe
};

//GPS targets as arguments from user
int main(int argc, char* argv[]) {

    /*if(argc < 3 || argc > 3)
    {
        cout << "Right usage: <executable progam>, <X1>, <Y1>,..." << endl;
    }
    if(argc == 3)
    {
        gps_pos.x = stof(argv[1]);
        gps_pos.y = stof(argv[2]);
        cout << "I took coordinates: " << gps_pos.x << " " << gps_pos.y << endl;
    }*/

    //Default τιμές
    unsigned long long seed = static_cast<unsigned long long>(chrono::system_clock::now().time_since_epoch().count()); 
    unsigned int dimX = 40;
    unsigned int dimY = 40;
    unsigned int numMovingCars = 3;
    unsigned int numMovingBikes = 4;
    unsigned int numParkedCars = 5;
    unsigned int numStopSigns = 2;
    unsigned int numTrafficLights = 2;
    unsigned int simulationTicks = 100;
    unsigned int minConfidenceThreshold = 40;
    vector<Position> gpsTargets;


    //Prints για βοηθεια στην εκτέλεση
    auto printHelp () {
    cout << "Self −Driving Car Simulation" << endl;
    cout << "Usage: " << endl;
    cout << " −−seed <n>   <<Random seed ( default : current time)" << endl;
    cout << " −−dimX <n>   <<World width ( default : 40)" << endl;
    cout << " −−dimY <n>   <<World height ( default : 40)" << endl;
    cout << " −−numMovingCars <n>   <<Number of moving cars ( default : 3)" << endl;
    cout << " −−numMovingBikes <n>   <<Number of moving bikes ( default : 4)" << endl;
    cout << " −−numParkedCars <n>   <<Number of parked cars ( default : 5)" << endl;
    cout << " −−numStopSigns <n>   <<Number of stop signs (default : 2)" << endl;
    cout << " −−numTrafficLights <n>   <<Number of traffic lights ( default : 2)" << endl;
    cout << " −−simulationTicks <n>   <<Maximum simulation ticks ( default : 100)" << endl;
    cout << " −−minConfidenceThreshold <n> Minimum confidence cutoff ( default : 40)" << endl;
    cout << " −−gps <x1> <y1> [x2 y2 . . .] GPS target coordinates (required)" << endl;
    cout << " −−help
    Show this help message" << endl << endl;
    cout << "Example usage: " << endl;
    cout << " ./ oopproj_2025 −−seed 12 −−dimY 50 −−gps 10 20 32 15" << endl;
    }
    
    // επεξεργασία arguments
    for(int i = 1; i < argc; ++i) {
        string a = argv[i];
        
        if(a == --help){
            printHelp();
            return 0;
        } else if(a == "--seed") {
            if (i + 1 >= argc) {
                cerr << "--seed requires a value\n";
                return 1;
            } 
            try { seed = stoull(argv[++i]); } 
            catch (const invalid_argument&) {
                cerr << "Invalid value for --seed\n";
                return 1;
            }
        } else if(a == "--dimX") {
            if (i + 1 >= argc) {
                cerr << "--dimX requires a value\n";
                return 1;
            } 
            try { dimX = stoull(argv[++i]); } 
            catch (const invalid_argument&) {
                cerr << "Invalid value for --dimX\n";
                return 1;
            }
        } else if(a == "--dimY") {
            if (i + 1 >= argc) {
                cerr << "--dimY requires a value\n";
                return 1;
            } 
            try { dimY = stoull(argv[++i]); } 
            catch (const invalid_argument&) {
                cerr << "Invalid value for --dimY\n";
                return 1;
            }
        } else if(a == "--numMovingCars") {
            if (i + 1 >= argc) {
                cerr << "--numMovingCars requires a value\n";
                return 1;
            } 
            try { numMovingCars = stoull(argv[++i]); } 
            catch (const invalid_argument&) {
                cerr << "Invalid value for --numMovingCars\n";
                return 1;
            }
        } else if(a == "--numMovingBikes") {
            if (i + 1 >= argc) {
                cerr << "--numMovingBikes requires a value\n";
                return 1;
            } 
            try { numMovingBikes = stoull(argv[++i]); } 
            catch (const invalid_argument&) {
                cerr << "Invalid value for --numMovingBikes\n";
                return 1;
            }
        } else if(a == "--numParkedCars") {
            if (i + 1 >= argc) {
                cerr << "--numParkedCars requires a value\n";
                return 1;
            } 
            try { numParkedCars = stoull(argv[++i]); } 
            catch (const invalid_argument&) {
                cerr << "Invalid value for --numParkedCars\n";
                return 1;
            }
        } else if(a == "--numStopSigns") {
            if (i + 1 >= argc) {
                cerr << "--numStopSigns requires a value\n";
                return 1;
            } 
            try { numStopSigns = stoull(argv[++i]); } 
            catch (const invalid_argument&) {
                cerr << "Invalid value for --numStopSigns\n";
                return 1;
            }
        } else if(a == "--numStopSigns") {
            if (i + 1 >= argc) {
                cerr << "--numStopSigns requires a value\n";
                return 1;
            } 
            try { numStopSigns = stoull(argv[++i]); }
            catch (const invalid_argument&) {
                cerr << "Invalid value for --numStopSigns\n";
                return 1;
            }
        } else if(a == "--numTrafficLights") {
            if (i + 1 >= argc) {
                cerr << "--numTrafficLights requires a value\n";
                return 1;
            } 
            try { numTrafficLights = stoull(argv[++i]); }
            catch (const invalid_argument&) {
                cerr << "Invalid value for --numTrafficLights\n";
                return 1;
            }
        } else if(a == "--simulationTicks") {
            if (i + 1 >= argc) {
                cerr << "--simulationTicks requires a value\n";
                return 1;
            } 
            try { simulationTicks = stoull(argv[++i]); } 
            catch (const invalid_argument&) {
                cerr << "Invalid value for --simulationTicks\n";
                return 1;
            }
        } else if(a == "--minConfidenceThreshold") {
            if (i + 1 >= argc) {
                cerr << "--minConfidenceThreshold requires a value\n";
                return 1;
            } 
            try { minConfidenceThreshold = stoull(argv[++i]); }
            catch (const invalid_argument&) {
                cerr << "Invalid value for --minConfidenceThreshold\n";
                return 1;
            }
        } // else gps targets one or more pairs
        else if(a == "--gps") {
            // collect trailing tokens until next --flag or end
            int j = i + 1;
            vector<string> tokens;
            while (j < argc && string(argv[j]).rfind("--", 0) != 0) {
                tokens.push_back(argv[j++]);
            }
            //need at least one pair
            if (tokens.empty()) { cerr << "--gps requires at least one pair of coordinates\n"; return 1; }
            if (tokens.size() % 2 != 0) { cerr << "--gps requires x y pairs (even number of values)\n"; return 1; }
            // Convert token pairs to Position objects and store them
            for (size_t k = 0; k < tokens.size(); k += 2) {
                try {
                    int x = stoi(tokens[k]);
                    int y = stoi(tokens[k + 1]);
                    gpsTargets.push_back(Position{x, y});
                } catch (...) { cerr << "Invalid GPS coordinates\n"; return 1; }
            }
            i = j - 1; // advance main loop to last processed GPS token
        } else {
            // Any unknown flag triggers an error and prints help
            cerr << "Unknown argument: " << a << "\n";
            printHelp();
            return 1;
        }

    }

    return 0;
}