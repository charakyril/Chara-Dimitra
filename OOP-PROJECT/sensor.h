
#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <string>
#include "types.h"
#include "sensor_reading.h"
#include "world.h"
//#include <cmath>

using namespace std;
// Global sensor ID counter
static unsigned int sensorIdCounter = 0;

class Sensors 
{
    public:
        //Sensor ID
        unsigned int id;
        //lidar, radar, camera
        string SENS_TYPE;
        //maximum detection range
        unsigned int RANGE;
        //optiko pedio
        unsigned int VIS_RANGE;
        //accuracy of sensor for distance
        int DISTANCE_ACCURACY;
        //accuracy of sensor for object category
        int CATEGORY_ACCURACY;
        //Car position
        Position car_position;
        //Each sensor returns a sensor reading
        SensorReading sensor_reading;

        //Constructor
        Sensors(const string& sens_type, unsigned int range, unsigned int vis_range, int dist, int category,
        Position car_pos)
        : id(++sensorIdCounter), SENS_TYPE(sens_type), RANGE(range), VIS_RANGE(vis_range), DISTANCE_ACCURACY(dist), CATEGORY_ACCURACY(category),
        car_position(car_pos) {}

        //Destructor
        virtual ~Sensors() 
        {
            //cout << "Sensor destroyed. No further data from me!" << endl;
        }

        //Pure virtual getDistance
        virtual void getDistance() const = 0;
        
        //Pure virtual getConfidence
        virtual void getConfidence() const = 0;
        
        //Pure virtual getSensorReading
        virtual SensorReading getSensorReading() const = 0;
};

//----SENSOR LIDAR----//
class Lidar : public Sensors
{
    public:
        float ACCURACY;
        //Constructor
        Lidar(const string& sens_type, unsigned int range, unsigned int vis_range, int dist, int category, 
        Position car_pos, float accuracy)
        : Sensors(sens_type, range, vis_range, dist, category, car_pos), ACCURACY(accuracy)
        {
            cout << "[+LIDAR:" << id << "] Lidar sensor ready - Sensing with pew pews!" << endl;
            //Initiallizing sensor reading
            /*sensor_reading.objectId = WorldObject.getID();
            sensor_reading.type = WorldObject.getType();
            sensor_reading.position.x = -1;
            sensor_reading.position.y = -1;
            sensor_reading.speed = "N/A";
            sensor_reading.direction.x = 0.0;
            sensor_reading.direction.y = 0.0;
            sensor_reading.signText = "N/A";
            sensor_reading.lightColour = "N/A";*/
            sensor_reading.objectId = "N/A";
            sensor_reading.type = "N/A";
            sensor_reading.position.x = -1;
            sensor_reading.position.y = -1;
            sensor_reading.distance = -1.0f;
            sensor_reading.confidence = 0.0f;
            sensor_reading.speed = "N/A";
            sensor_reading.direction.x = 0.0;
            sensor_reading.direction.y = 0.0;
            sensor_reading.signText = "N/A";
            sensor_reading.lightColour = "N/A";
        }
        //Destructor
        ~Lidar() override {
            // No output on destruction
        }

        //Returns distance
        void getDistance() const override
        {
            //using getPosition from worldobject
           // Position object_position = WorldObject.getPosition();
            //Store in sesnor reading
            //sensor_reading.distance = (abs(object_position.x - car_position.x) + abs(object_position.y - car_position.y));
        }

        //Returns certainty
        void getConfidence() const override
        {
           // sensor_reading.confidence = (((DISTANCE_ACCURACY + CATEGORY_ACCURACY)/2)/100);
        }

        //Sensor Reading of each sensor
        SensorReading getSensorReading() const override
        {
            return sensor_reading;
        }
};

//----SENSOR RADAR----//
class Radar : public Sensors
{
    public:
        Direction facingDirection;
        float ACCURACY;
        
        //Constructor
        Radar(const string& sens_type, unsigned int range, unsigned int vis_range, int dist, int category,
        Position car_pos, Direction facing, float accuracy)
        : Sensors(sens_type, range, vis_range, dist, category, car_pos), facingDirection(facing), ACCURACY(accuracy)
        {
            cout << "[+RADAR:" << id << "] Radar sensor ready - I'm a Radio star!" << endl;
            //Initiallizing with default arguments the ones that radar does not return
           /*sensor_reading.type = WorldObject.getID();
            sensor_reading.position.x = -1;
            sensor_reading.position.y = -1;
            sensor_reading.speed = MovingObjects.getSpeed();
            sensor_reading.direction = MovingObjects.getDirection();
            sensor_reading.signText = "N/A";
            sensor_reading.lightColour = "N/A";*/
            sensor_reading.objectId = "N/A";
            sensor_reading.type = "N/A";
            sensor_reading.position.x = -1;
            sensor_reading.position.y = -1;
            sensor_reading.distance = -1.0f;
            sensor_reading.confidence = 0.0f;
            sensor_reading.speed = "N/A";
            sensor_reading.direction.x = 0.0;
            sensor_reading.direction.y = 0.0;
            sensor_reading.signText = "N/A";
            sensor_reading.lightColour = "N/A";
        }
        //Destructor
        ~Radar() override {
           // No output on destruction
        }

        //Returns distance
        void getDistance() const override
        {
            //Detects only moving objects so, using getPosition from moving objects
           //Position object_position = MovingObjects.getPosition();
            //store in sesnor reading
            //sensor_reading.distance = (abs(object_position.x - car_position.x) + abs(object_position.y - car_position.y));
        }

        //Returns certainty
        void getConfidence() const override
        {
            //sensor_reading.confidence = (((DISTANCE_ACCURACY + CATEGORY_ACCURACY)/2)/100);
        }

        //Sensor Reading of each sensor
        SensorReading getSensorReading() const override
        {
            return sensor_reading;
        }
};

//----CAMERA SENSOR----//
class Camera : public Sensors
{
    public:
        Position position;  // Camera's position in the world
        string status;
        Direction facingDirection;
        string lastSignText;
        string lastLightColor;
        float ACCURACY;

        //Constructor
        Camera(const string& sens_type, unsigned int range, unsigned int vis_range, int dist, int category, 
        Position car_pos, const string& cam_status, float accuracy, const string& speed, Direction facing,
        const string& signText, const string& lightColor)
        : Sensors(sens_type, range, vis_range, dist, category, car_pos), status(cam_status), 
          facingDirection(facing), lastSignText(signText), lastLightColor(lightColor), ACCURACY(accuracy)
        {
            cout << "[+CAMERA:" << id << "] Camera sensor ready - Say cheese!" << endl;
            //Store values in sensor reading
            /*sensor_reading.position = WorldObject.getPosition();
            sensor_reading.type = WorldObject.getType();
            sensor_reading.objectId = WorldObject.getID();
            sensor_reading.speed = MovingObjects.getSpeed();
            sensor_reading.direction = MovingObjects.getDirection();
            sensor_reading.signText = TRAFFIC_SIGNS.getSignText();
            sensor_reading.lightColour = TRAFFIC_LIGHTS.getColour();*/
             position = car_pos;
            //Initialize sensor reading
            sensor_reading.objectId = "N/A";
            sensor_reading.type = "N/A";
            sensor_reading.position.x = -1;
            sensor_reading.position.y = -1;
            sensor_reading.distance = -1.0f;
            sensor_reading.confidence = 0.0f;
            sensor_reading.speed = speed;
            sensor_reading.direction = facing;
            sensor_reading.signText = signText;
            sensor_reading.lightColour = lightColor;
        } 
        //Destructor
        ~Camera() override {
            // No output on destruction
        }

        //Distance from object
        void getDistance() const override
        {
            // Manhattan distance; you can add noise if needed
           // Position object_position = WorldObject.getPosition();
           // sensor_reading.distance = (abs(object_position.x - car_position.x) + abs(object_position.y - car_position.y)); //noise + 0,05
        }

        //Finds certainty
        void getConfidence() const override
        {
           // sesnor_reading.confidence = (((DISTANCE_ACCURACY + CATEGORY_ACCURACY)/2)/100);
        }

        //Sensor Reading of each sensor
        SensorReading getSensorReading() const override
        {
            return sensor_reading;
        }
};

// Static member definition

#endif // SENSOR_H
/*

#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <string>
#include "types.h"
#include "world.h"
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
        Sensors(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy)
        : SENS_TYPE(sens_type), RANGE(range), VIS_RANGE(vis_range), ACCURACY(accuracy) {}

        //Destructor
        virtual ~Sensors() 
        {
            //cout << "Sensor destroyed. No further data from me!" << endl;
        }
};

//----SENSOR LIDAR----//
class Lidar : public Sensors
{
    public:
        float distance;
        string object_type_detect;
        float sureness;
    
        //Constructor
        Lidar(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy,
        float dist, const string& detect_object, float sure)
        : Sensors(sens_type, range, vis_range, accuracy), distance(dist), object_type_detect(detect_object), sureness(sure) 
        {
            //cout << "Lidar sensor ready - Sensing with pew pews!" << endl;
        }

        //Destructor
        ~Lidar() override {}
};

//----SENSOR RADAR----//
class Radar : public Sensors
{
    public:
        float distance;
        //SPEED = STOPPED, HALF_SPEED, FULL_SPEED
        string speed;
        Direction movement_direction;
        float sureness;
    
        //Constructor
        Radar(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy,
        float dist, const string& sp, Direction dir, float sure)
        : Sensors(sens_type, range, vis_range, accuracy), distance(dist), speed(sp), movement_direction(dir), sureness(sure) 
        {
            //cout << "Radar sensor ready - I'm a Radio star!" << endl;
        }

        //Destructor
        ~Radar() override {}
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
        //position of car
        Direction movement_direction;
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
            //cout << "Camera sensor ready - Say cheese!" << endl;
        }

        //Destructor
        ~Camera() override {}

        //distance from object
        int distance(Position position, Direction movement_direction) const
        {
            return (abs(position.x - movement_direction.x) + (abs(position.y - movement_direction.y)));
        }
        //distance from gps target
        int gps_distance(Position gps_pos, Direction movement_direction) const
        {
            return (abs(gps_pos.x - movement_direction.x) + (abs(gps_pos.y - movement_direction.y)));
        }
};

// Static member definition
unsigned int Sensors::sensorIdCounter = 0;

#endif // SENSOR_H */