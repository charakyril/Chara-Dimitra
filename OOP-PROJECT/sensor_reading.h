//DONE SD

#ifndef SENSOR_READING_H
#define SENSOR_READING_H

#include <string>
#include "types.h"

using namespace std;

struct SensorReading
{
    string objectId;   //object identity string
    string type; // object category 
    Position position{0, 0};  //best position with default initialization
    float distance = -1.0f; //MANHATTAN DISTANCE
    float confidence = 0.0f;  
    string speed; // speed string (if available)
    Direction direction{0.0f, 0.0f}; //movement direction with default initialization
    string signText; //signs
    string lightColour; //traffic lights colout
};

#endif // SENSOR_READING_H
