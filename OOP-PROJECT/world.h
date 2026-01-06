//DONE
#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <string>
#include <random>
#include "types.h"
using namespace std;

//MAKE GLOBAL COUNT VARIABLES FOR ID
//Ορίζονται ως inline για να μην υπάρχουν πολλαπλοί ορισμοί τους
/*inline int count_cars = 0;
inline int count_bikes = 0;
inline int count_st_v = 0;
inline int count_traf_signs = 0;
inline int count_traf_lights = 0;*/

static unsigned int carIdCounter = 0;
static unsigned int bikeIdCounter = 0;
static unsigned int parkedIdCounter = 0;
static unsigned int lightIdCounter = 0;
static unsigned int signIdCounter = 0;

//Make Objects class
class WorldObject 
{
    protected: 
        string ID;
        char GLYPH;
        string TYPE;
        Position POSITION;
    public:
        //Constructor
        WorldObject(const char& glyph, const string& type, float x, float y, unsigned int id)
        : GLYPH(glyph), TYPE(type)
        {
            POSITION.x = x;
            POSITION.y = y;  
            ID = type + ":" + to_string(id);
        }
        //Destructor
        virtual ~WorldObject() {}
        
        //Virtual describe function for polymorphism
        virtual void describe() const = 0;

        // Accessors
        Position getPosition() const { return POSITION; }
        void setPosition(int x, int y) { POSITION.x = x; POSITION.y = y; }
        string getID() const { return ID; }
        string getType() const { return TYPE; }
        char getGlyph() const { return GLYPH; }

        // Update per tick; default no-op
        virtual void updateTick(unsigned int /*tick*/, unsigned int /*dimX*/, unsigned int /*dimY*/) {}
};

//Generative class 1 of Objects//
class MovingObjects : public WorldObject
{
    protected:
        //SPEED = STOPPED, HALF_SPEED, FULL_SPEED
        string SPEED;
        Direction DIRECTION;
        //Car or bicycle
        string object_type;
    public:
        //Constructor - id parameter removed (not needed from derived classes)
        MovingObjects(const char& glyph, const string& type, float x, float y, 
        const string& speed, Direction direction, const string& obj_type, unsigned int* customCounter = nullptr)
        : WorldObject(glyph, type, x, y, customCounter ? ++(*customCounter) : ++carIdCounter), SPEED(speed), DIRECTION(direction), object_type(obj_type) 
        {
            // Debug output removed - only derived class output messages are shown
        }

        //Destructor
        virtual ~MovingObjects() {}

        //Describe function
        virtual void describe() const = 0;

        // Update position based on speed and direction
        void updateTick(unsigned int /*tick*/, unsigned int dimX, unsigned int dimY) override
        {
            int steps = 0;
            if (SPEED == "FULL_SPEED") steps = 2;
            else if (SPEED == "HALF_SPEED") steps = 1;
            else steps = 0;

            for (int i = 0; i < steps; ++i) {
                int nx = POSITION.x + static_cast<int>(DIRECTION.x);
                int ny = POSITION.y + static_cast<int>(DIRECTION.y);
                // If moved outside bounds, mark as out-of-bounds with negative coordinates
                if (nx < 0 || ny < 0 || static_cast<unsigned int>(nx) >= dimX || static_cast<unsigned int>(ny) >= dimY) {
                    POSITION.x = -1; POSITION.y = -1; // indicate removal
                    return;
                }
                POSITION.x = nx; POSITION.y = ny;
            }
        }

        //Getter function for speed
        //virtual string getSpeed() const = 0;
};

//GENERATIVE CLASSES FOR MOVING OBJECTS//

class CARS : public MovingObjects
{
    public:
        //Constructor
        CARS(const char& glyph, const string& type, float x, float y,
        const string& speed, Direction direction, const string& obj_type)
        : MovingObjects(glyph, type, x, y, speed, direction, obj_type)
        {
           // count_cars++;
            //ID = object_type.append(to_string(count_cars));
            //cout << "Object has id: " << ID << endl;
            // Direction to string for logging
            string dirStr;
            if (direction.x == 1) dirStr = "EAST";
            else if (direction.x == -1) dirStr = "WEST";
            else if (direction.y == 1) dirStr = "NORTH";
            else dirStr = "SOUTH";
            
            cout << "[+CAR:" << ID << "] Initialized at (" << x << "," << y << ") facing " 
                 << dirStr << " - No driver's license required!" << endl;
        }
        //Destructor
        ~CARS() override 
        { 
           // count_cars--;
            //cout << "Being scrapped..." << endl;
            cout << "[-CAR:" << ID << "] Our journey is complete!" << endl;
        }
        void describe() const override
        {
            cout << "Moving object is of type: " << object_type << " and has speed: " << SPEED << endl;
            cout << "This car has id: " << ID << endl;
        }
        
        //Getter function for speed
       //string getSpeed() const override { return SPEED; }
};

class BIKES : public MovingObjects
{
    public:
        //Constructor
        BIKES(const char& glyph, const string& type, float x, float y, 
        const string& speed, Direction direction, const string& obj_type)
        : MovingObjects(glyph, type, x, y, speed, direction, obj_type, &bikeIdCounter)
        {
            //count_bikes++;
            //ID = object_type.append(to_string(count_bikes));
            //cout << "Object has id: " << ID << endl;
            cout << "[+BIKE:" << ID << "] Initialized at (" << x << "," << y << ")" << endl;
        }
        //Destructor
        ~BIKES() override 
        { 
           // count_bikes--; 
            cout << "[-BIKE:" << ID << "] Being locked away ..." << endl;
        }
        void describe() const override
        {
            cout << "Moving object is of type: " << object_type << " and has speed: " << SPEED << endl;
            cout << "This bike has id: " << ID << endl;
        }
        //Getter function for speed
        //string getSpeed() const override { return SPEED; }
};

//Generative class 2 of Objects//
class StaticObjects : public WorldObject
{
    protected:
        string object_type_s;
    public:
        //Constructor
        StaticObjects(const char& glyph, const string& type, float x, float y, const string& obj_type_s, unsigned int* customCounter = nullptr)
        : WorldObject(glyph, type, x, y, customCounter ? ++(*customCounter) : ++parkedIdCounter), object_type_s(obj_type_s) {}
        
        //Destructor
        virtual ~StaticObjects() {}  
        virtual void describe() const = 0;
};

//GENERATIVE CLASSES OF STATIC OBJECTS//

class STAT_VEH : public StaticObjects
{
    public:
        //Constructor
        STAT_VEH(const char& glyph, const string& type, float x, float y, const string& obj_type_s)
        : StaticObjects(glyph, type, x, y, obj_type_s)
        {
            //ID = object_type_s.append(to_string(count_st_v));
            //cout << ID << " is parked at (" << POSITION.x << ")" << "," <<" (" << POSITION.y <<")" << endl;
            //count_st_v++;
            //cout << "Object id: " << ID << endl;
            cout << "[+PARKED:" << ID << "] Parked at (" << x << "," << y << ")" << endl;
        } 
        //Destructor
        ~STAT_VEH() override 
        { 
            //count_st_v--;
            cout << "[-PARKED:" << ID << "] I'm being towed away!" << endl;
            //cout << "I'm being towed away!" << endl;
        }
        void describe() const override
        {
            cout << "Static object is of type: " << object_type_s << " and is at position: " << POSITION.x << " " << POSITION.y << endl;
            cout << "This static vehicle has id: " << ID << endl;
        } 
};

class TRAFFIC_SIGNS : public StaticObjects
{
    private: 
        string ID_TEXT;
    public: 
        //Constructor
        TRAFFIC_SIGNS(const char& glyph, const string& type, float x, float y, const string& obj_type_s, 
        const string& id_text)
        : StaticObjects(glyph, type, x, y, obj_type_s, &signIdCounter), ID_TEXT(id_text)
        {
            //count_traf_signs++;
            //ID = object_type_s.append(to_string(count_traf_signs));
            //cout << " Object id: " << ID << endl;
            cout << "[+SIGN:" << ID << "] Initialized at (" << x << "," << y << ") as " << id_text << endl; 
        }
        //Destructor
        ~TRAFFIC_SIGNS() override { 
            //count_traf_signs--;
            cout << "[-SIGN:" << ID << "] Being removed" << endl; 
        }

         void describe() const override
        {
            cout << "Static object is of type: " << object_type_s << endl;
            cout << "This traffic sign has id: " << ID << " and is a " << ID_TEXT << " sign" <<  endl;
        }
        
        //Getter function
        string getSignText() const { return ID_TEXT; }
};

class TRAFFIC_LIGHTS : public StaticObjects
{
     private:
        unsigned int initialTick;
        string COLOUR;
    public:
        //Constructor - with random initial state option
        TRAFFIC_LIGHTS(const char& glyph, const string& type, float x, float y, const string& obj_type_s, 
        const string& colour, bool randomInitial = true, unsigned int tick = 0)
        : StaticObjects(glyph, type, x, y, obj_type_s), initialTick(tick), COLOUR(colour)
        {
            ID = obj_type_s + ":" + to_string(++lightIdCounter);
            
            if (randomInitial && initialTick == 0) {
                // Random initial state
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> distrib(0, 13);
                initialTick = distrib(gen);
                
                unsigned int cycle = initialTick % 14u;
                if (cycle < 4u) COLOUR = "RED";
                else if (cycle < 12u) COLOUR = "GREEN";
                else COLOUR = "YELLOW";
            }
            
            cout << "[+LIGHT:" << ID << "] Initialized at (" << x << "," << y << ") to " << COLOUR << endl;
        }
        //Destructor
        ~TRAFFIC_LIGHTS() override
        {
            cout << "[-LIGHT:" << ID << "] Turning off" << endl;
        }
        //Describe
        void describe() const override
        {
            cout << "Static object is of type: " << object_type_s << endl;   
            cout << "This traffic light has id: " << ID << " and has colour: " << COLOUR << endl;
        }

        // Update light colour based on tick (RED 4, GREEN 8, YELLOW 2)
        void updateTick(unsigned int tick, unsigned int /*dimX*/, unsigned int /*dimY*/) override
        {
            unsigned int effectiveTick = initialTick + tick;
            unsigned int cycle = effectiveTick % 14u;
            if (cycle < 4u) COLOUR = "RED";
            else if (cycle < 12u) COLOUR = "GREEN";
            else COLOUR = "YELLOW";
        }

        string getColour() const { return COLOUR; }
};

#endif // WORLD_H