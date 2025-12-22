#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
#include "types.h"
#include "world.h"
#include "sensor.h"

using namespace std;

//Global variable ticks//
unsigned int ticks = 0;
//Global variable gps_pos//
Position gps_pos;

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
        void Decelerate() const
        {
            if((cam_sensor.LightColour == "RED" || cam_sensor.LightColour == "YELLOW") && 
            (cam_sensor.distance(cam_sensor.position, car_direction) <= 3))
            {
                if(speed == "FULL_SPEED")
                    speed == "HALF_SPEED";
                if(speed == "HALF_SPEED")
                    speed == "STOPPED";
            }
            if(cam_sensor.gps_distance(cam_sensor.position, car_direction) <= 5)
            {
                if(speed == "FULL_SPEED")
                    speed == "HALF_SPEED";
                if(speed == "HALF_SPEED")
                    speed == "STOPPED";
            }
            //Need to do global variable gps_pos
            if(cam_sensor.distance(gps_pos, car_direction) <= 2)
            {
                if(speed == "FULL_SPEED")
                    speed == "HALF_SPEED";
                if(speed == "HALF_SPEED")
                    speed == "STOPPED";
            }
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
        gps_pos.x = stof(argv[1]);
        gps_pos.y = stof(argv[2]);
        cout << "I took coordinates: " << gps_pos.x << " " << gps_pos.y << endl;
    }

    return 0;
}