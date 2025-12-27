
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

        //Constructor
        Sensors(const string& sens_type, unsigned int range, unsigned int vis_range, Direction dir, Position pos, Position car_pos)
        : SENS_TYPE(sens_type), RANGE(range), VIS_RANGE(vis_range), ACCURACY(accuracy)
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
        float distance;
        float sureness;
        
        //Constructor
        Lidar(const string& sens_type, unsigned int range, unsigned int vis_range, 
        const string& detect_object, float dist, float sure)
        : Sensors(sens_type, range, accuracy), distance(dist), object_type_detect(detect_object), sureness(sure)
        {

            cout << "One working lidar sensor\n"
        }
        //Destructor
        ~Lidar() override
        {
            cout << "Lidar sensor stopped operating\n";
        }
        /*Distance from object
        void distance_sensor() const override
        {
            return ((abs(position.x - car_position.x) + (abs(position.y - car_position.y))) + 0.05); //noise
        }
        //Distance from gps target
        int GPS_distance(Position gps_pos) const
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
        }*/
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
        Direction movement_direction;
    
        //Constructor
        Radar(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy, float dist, const string& sp, 
        float sure, Direction dir)
        : Sensors(sens_type, range, vis_range, dir, pos, car_pos), speed(sp), sureness(sure)
        {
            cout << "One working radar sensor\n";
            //Find distance
        }
        //Destructor
        ~Radar() override
        {
            cout << "Radar sensor stopped operating\n";
        }
        /*/Distance from object
        int distance() const
        {
            return (abs(position.x - car_position.x) + (abs(position.y - car_position.y)) + 0.05); //noise
        }
        //Distance from gps target
        int GPS_distance(Position gps_pos) const
        {
            return (abs(gps_pos.x - car_position.x) + (abs(gps_pos.y - car_position.y)) + 0.05); //noise
        }
        //Returns certainty
        float Confidence()
        {
            //Confidence = (akribeia kathe metrhshs / plithos metrhsewn(2)) / 100
            float sureness = ((DISTANCE_ACCURACY + CATEGORY_ACCURACY)/2)/100;
            return sureness;
        }*/
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
        //position of object
        Position position;
        string ObjectID;
        float sureness;
        //SPEED = STOPPED, HALF_SPEED, FULL_SPEED
        string speed;
        //Signs can be STOP, YIELD etc
        string SignText;
        //GREEN, YELLOW, RED    
        string LightColour;

        //Constructor
        Camera(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy,
        const string& detect_object, Position pos, const string& obj_id, float sure, const string& sp,
        Direction dir, const string& signtext, const string& lightcolour)
        : Sensors(sens_type, range, vis_range, accuracy), object_type_detect(detect_object),position(pos), ObjectID(obj_id),
        sureness(sure), speed(sp), movement_direction(dir), SignText(signtext), LightColour(lightcolour)
        {
            cout << "One camera sensor working\n";
            
        } 
        //Destructor
        ~Camera() override
        {
            cout << "Camera sensor stopped working\n";
        }
        //distance from object
        int distance(Position position, Direction movement_direction) const
        {
            return (abs(position.x - movement_direction.x) + (abs(position.y - movement_direction.y))); //noise + 0,05
        }
        //distance from gps target
        int GPS_distance(Position gps_pos, Direction movement_direction) const
        {
            return (abs(gps_pos.x - movement_direction.x) + (abs(gps_pos.y - movement_direction.y))); //noise + 0,05
        }
        /*/Returns certainty
        float Confidence()
        {
            //Confidence = (akribeia kathe metrhshs / plithos metrhsewn(2)) / 100
            float sureness = ((DISTANCE_ACCURACY + CATEGORY_ACCURACY)/2)/100;
            return sureness;
        }
        /*/Describe
        void describe_sensor() const override
        {
            cout << "Sensor is of type: " << SENS_TYPE << endl;
        }
};

#endif // SENSOR_H