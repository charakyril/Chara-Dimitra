
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
        : id(++sensorIdCounter), SENS_TYPE(sens_type), RANGE(range), VIS_RANGE(vis_range),
        DISTANCE_ACCURACY(dist), CATEGORY_ACCURACY(category), car_position(car_pos) {}

        //Destructor
        virtual ~Sensors() {}
        
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
        ~Lidar() override {}

        //Sensor Reading of each sensor
        SensorReading getSensorReading() const override { return sensor_reading; }

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
        ~Radar() override {}

        //Sensor Reading of each sensor
        SensorReading getSensorReading() const override { return sensor_reading; }

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
        ~Camera() override {}

        //Sensor Reading of each sensor
        SensorReading getSensorReading() const override { return sensor_reading; }

};

#endif //SENSOR_H