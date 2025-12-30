//DONE

#ifndef SENSOR_READING_H
#define SENSOR_READING_H

#include <string>
#include "types.h"
using namespace std;

struct SensorReading
{
    string objectId;   //object identity string
    string type; // object category 
    Position position;  //best position
    float distance = -1.0f; //MANHATTAN DISTANCE
    float confidence = 0.0f;  
    string speed; // speed string (if available)
    Direction direction; //movement direction
    string signText; //signs
    string lightColour; //traffic lights colout
};

#endif // SENSOR_READING_H