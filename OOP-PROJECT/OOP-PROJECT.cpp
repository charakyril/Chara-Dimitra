#include <iostream>
#include <string>
#include <cctype>
#include "types.h"
#include "world.h"
#include "sensor.h"

using namespace std;

//Global variable ticks//
unsigned int ticks = 0;

<<<<<<< HEAD
/*void printHelp ()
{
    cout << "Self −Driving Car Simulation" << endl;
    cout << "Usage: " << endl;
    cout << " −−seed <n>
    Random seed ( default : current time)" << endl;
    cout << " −−dimX <n>
    cout << " −−dimY <n>
    cout << " −−numMovingCars <n>
    cout << " −−numMovingBikes <n>
    cout << " −−numParkedCars <n>
    cout << " −−numStopSigns <n>
    cout << " −−numTrafficLights <n>
    cout << " −−simulationTicks <n>
    World width ( default : 40)" << endl;
    World height ( default : 40)" << endl;
    Number of moving cars ( default : 3)" << endl;
    Number of moving bikes ( default : 4)" << endl;
    Number of parked cars ( default : 5)" << endl;
    Number of stop signs (default : 2)" << endl;
    Number of traffic lights ( default : 2)" << endl;
    Maximum simulation ticks ( default : 100)" << endl;
    cout << " −−minConfidenceThreshold <n> Minimum confidence cutoff ( default : 40)" << endl;
    cout << " −−gps <x1> <y1> [x2 y2 . . .] GPS target coordinates (required)" << endl;
    cout << " −−help
    Show this help message" << endl << endl;
    cout << "Example usage: " << endl;
    cout << " ./ oopproj_2025 −−seed 12 −−dimY 50 −−gps 10 20 32 15" << endl;
}*/
=======

// Shared types (Position, Direction) are defined in types.h

>>>>>>> 4bcc5168117f1e89d4e1d4706cc1b970e68e1360

//Class for NavigationSystem
class NavigationSystem
{
    private:
        //GPS targets
        float gps_x;
        float gps_y;
    public:
        //Constructor
        //Destructor
        //fuseSensorData with SensorFusionEngine and MakeDesicion to move
};


//----CLASS FOR SelfDrivingCar----//
class SelfDrivingCar
{
    protected:
        Direction car_direction;
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
        SelfDrivingCar(Direction dir, const string& sp, Lidar lidar, Radar radar, Camera camera, NavigationSystem nav)
        : car_direction(dir), speed(sp), ld_sensor(lidar), rd_sensor(radar), cam_sensor(camera), nav_system(nav)
        {
            cout << "I made the self driving car\n";
        }
        //Destructor
        ~SelfDrivingCar() 
        {
            cout << "No self driving car\n";
        }

        //Describe
};


//GPS targets as arguments from user
int main(int argc, char* argv[])
{

    if(argc < 3 || argc > 3)
    {
        cout << "Right usage: <executable progam>, <X1>, <Y1>,..." << endl;
    }
    if(argc == 3)
    {
        Position start_pos;
        start_pos.x = stof(argv[1]);
        start_pos.y = stof(argv[2]);
        cout << "I took coordinates: " << start_pos.x << " " << start_pos.y << endl;
    }

    return 0;
}