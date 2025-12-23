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
        int DISTANCE_ACCURACY;
        int CATEGORY_ACCURACY;
        //Car direction
        Direction movement_direction;
        //Object position
        Position position;
        //Car position
        Position car_position;

        //Constructor
        Sensors(const string& sens_type, unsigned int range, unsigned int vis_range, Direction dir, Position pos, Position car_pos)
        : SENS_TYPE(sens_type), RANGE(range), VIS_RANGE(vis_range), movement_direction(dir), position(pos), car_position(car_pos)
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
        
        //Constructor
        Lidar(const string& sens_type, unsigned int range, unsigned int vis_range, Direction dir, Position pos, 
        Position car_pos, const string& detect_object)
        : Sensors(sens_type, range, vis_range, dir, pos, car_pos), object_type_detect(detect_object)
        {
            //Every sensor has it's own accuracy
            DISTANCE_ACCURACY = 99;
            CATEGORY_ACCURACY = 87;
            cout << "One working lidar sensor\n";
            //Find distance
        }
        //Destructor
        ~Lidar() override
        {
            cout << "Lidar sensor stopped operating\n";
        }
        //Distance from object
        int distance() const
        {
            return ((abs(position.x - car_position.x) + (abs(position.y - car_position.y))) + 0.05); //noise
        }
        //Distance from gps target
        int GPS_distance() const
        {
            return (abs(gps_pos.x - car_position.x) + (abs(gps_pos.y - car_position.y)) + 0.05); //noise
        }
        //Returns object category
        string Object_category() 
        {
            return object_type_detect;
        }
        //Returns certainty
        float Confidence()
        {
            //Confidence = (akribeia kathe metrhshs / plithos metrhsewn(2)) / 100
            float sureness = ((DISTANCE_ACCURACY + CATEGORY_ACCURACY)/2)/100;
            return sureness;
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
        Radar(const string& sens_type, unsigned int range, unsigned int vis_range, Direction dir, Position pos,  
        Position car_pos, const string& sp, float sure)
        : Sensors(sens_type, range, vis_range, dir, pos, car_pos), speed(sp), sureness(sure)
        {
            DISTANCE_ACCURACY = 95;
            CATEGORY_ACCURACY = 95;
            cout << "One working radar sensor\n";
            //Find distance
        }
        //Destructor
        ~Radar() override
        {
            cout << "Radar sensor stopped operating\n";
        }
        //Distance from object
        int distance() const
        {
            return (abs(position.x - car_position.x) + (abs(position.y - car_position.y)) + 0.05); //noise
        }
        //Distance from gps target
        int GPS_distance() const
        {
            return (abs(gps_pos.x - car_position.x) + (abs(gps_pos.y - car_position.y)) + 0.05); //noise
        }
        //Returns certainty
        float Confidence()
        {
            //Confidence = (akribeia kathe metrhshs / plithos metrhsewn(2)) / 100
            float sureness = ((DISTANCE_ACCURACY + CATEGORY_ACCURACY)/2)/100;
            return sureness;
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
        Camera(const string& sens_type, unsigned int range, unsigned int vis_range, Direction dir, Position pos,  
        Position car_pos, const string& detect_object, const string& obj_id, float sure, const string& sp, 
        const string& signtext, const string& lightcolour)
        : Sensors(sens_type, range, vis_range, dir, pos, car_pos), object_type_detect(detect_object), ObjectID(obj_id),
        sureness(sure), speed(sp), SignText(signtext), LightColour(lightcolour)
        {
            DISTANCE_ACCURACY = 95;
            CATEGORY_ACCURACY = 87;
            cout << "One camera sensor working\n";
            
        }       
        //Destructor
        ~Camera() override
        {
            cout << "Camera sensor stopped working\n";
        }
        //distance from object
        int distance() const
        {
            return (abs(position.x - car_position.x) + (abs(position.y - car_position.y)) + 0.05); //noise
        }
        //distance from gps target
        int GPS_distance() const
        {
            return (abs(gps_pos.x - car_position.x) + (abs(gps_pos.y - car_position.y)) + 0.05); //noise
        }
        //Returns certainty
        float Confidence()
        {
            //Confidence = (akribeia kathe metrhshs / plithos metrhsewn(2)) / 100
            float sureness = ((DISTANCE_ACCURACY + CATEGORY_ACCURACY)/2)/100;
            return sureness;
        }
        //Describe
        void describe_sensor() const override
        {
            cout << "Sensor is of type: " << SENS_TYPE << endl;
        }
};

#endif // SENSOR_H