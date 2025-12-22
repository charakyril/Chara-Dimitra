#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <string>
#include "types.h"
#include <cmath>

using namespace std;

//-----SENSORS-----//
class Sensors 
{
    public:
        string SENS_TYPE;
        unsigned int RANGE;
        unsigned int VIS_RANGE;
        int ACCURACY;
        //Car direction
        Direction movement_direction;
        //Object position
        Position position;
        //Car position
        Position car_position;

        //Constructor
        Sensors(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy, Direction dir, Position pos, Position car_pos)
        : SENS_TYPE(sens_type), RANGE(range), VIS_RANGE(vis_range), ACCURACY(accuracy), movement_direction(dir), position(pos), car_position(car_pos)
        {
            cout << "I just made a sensor\n";
        }
        //Destructor
        virtual ~Sensors()
        {
            cout << "I just destroyed a sensor\n";
        }
        //Pure virtual describe
        virtual void describe_sensor() const = 0;
};

//----SENSOR LIDAR----//
class Lidar : public Sensors
{
    public:
        string object_type_detect;
        float sureness;
        
        //Constructor
        Lidar(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy, Direction dir, Position pos, 
        Position car_pos, const string& detect_object, float sure)
        : Sensors(sens_type, range, vis_range, accuracy, dir, pos, car_pos), object_type_detect(detect_object), sureness(sure)
        {
            cout << "One working lidar sensor\n";
            //Find distance
        }
        //Destructor
        ~Lidar() override
        {
            cout << "Lidar sensor stopped operating\n";
        }
        //Distance from object
        int distance(Position pos, Position car_position) const
        {
            return (abs(position.x - car_position.x) + (abs(position.y - car_position.y)));
        }
        //Distance from gps target
        int gps_distance(Position gps_pos, Position car_position) const
        {
            return (abs(gps_pos.x - car_position.x) + (abs(gps_pos.y - car_position.y)));
        }

        //Describe
        void describe_sensor() const override
        {
            cout << "Sensor is of type: " << SENS_TYPE << endl;
        }
};

//----SENSOR RADAR----//
class Radar : public Sensors
{
    public:
        //SPEED = STOPPED, HALF_SPEED, FULL_SPEED
        string speed;
        float sureness;
    
        //Constructor
        Radar(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy, Direction dir, Position pos,  
        Position car_pos, const string& sp, float sure)
        : Sensors(sens_type, range, vis_range, accuracy, dir, pos, car_pos), speed(sp), sureness(sure)
        {
            cout << "One working radar sensor\n";
            //Find distance
        }
        //Destructor
        ~Radar() override
        {
            cout << "Radar sensor stopped operating\n";
        }
        //Distance from object
        int distance(Position pos, Position car_position) const
        {
            return (abs(position.x - car_position.x) + (abs(position.y - car_position.y)));
        }
        //Distance from gps target
        int gps_distance(Position gps_pos, Position car_position) const
        {
            return (abs(gps_pos.x - car_position.x) + (abs(gps_pos.y - car_position.y)));
        }
        //Describe
        void describe_sensor() const override
        {
            cout << "This sensor is of type: " << SENS_TYPE << endl;
        }
};

//----CAMERA SENSOR----//
class Camera : public Sensors
{
    public:
        string object_type_detect;
        string ObjectID;
        float sureness;
        //SPEED = STOPPED, HALF_SPEED, FULL_SPEED
        string speed;
        //Signs can be STOP, YIELD etc
        string SignText;
        //GREEN, YELLOW, RED    
        string LightColour;

        //Constructor
        Camera(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy, Direction dir, Position pos,  
        Position car_pos, const string& detect_object, const string& obj_id, float sure, const string& sp, 
        const string& signtext, const string& lightcolour)
        : Sensors(sens_type, range, vis_range, accuracy, dir, pos, car_pos), object_type_detect(detect_object), ObjectID(obj_id),
        sureness(sure), speed(sp), SignText(signtext), LightColour(lightcolour)
        {
            cout << "One camera sensor working\n";
            
        }       
        //Destructor
        ~Camera() override
        {
            cout << "Camera sensor stopped working\n";
        }
        //distance from object
        int distance(Position position, Position car_position) const
        {
            return (abs(position.x - car_position.x) + (abs(position.y - car_position.y)));
        }
        //distance from gps target
        int gps_distance(Position gps_pos, Position car_position) const
        {
            return (abs(gps_pos.x - car_position.x) + (abs(gps_pos.y - car_position.y)));
        }
        //Describe
        void describe_sensor() const override
        {
            cout << "Sensor is of type: " << SENS_TYPE << endl;
        }
};

#endif // SENSOR_H