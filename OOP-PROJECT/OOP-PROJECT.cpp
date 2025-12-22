#include <iostream>
#include <string>
#include <cctype>
#include "types.h"
#include "world.h"
#include "sensor.h"

using namespace std;

//Global variable ticks//
unsigned int ticks = 0;

// Shared types (Position, Direction) are defined in types.h


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