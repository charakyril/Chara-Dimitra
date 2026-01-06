//DONE
#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <string>
#include "types.h"
using namespace std;

//MAKE GLOBAL COUNT VARIABLES FOR ID
//Ορίζονται ως inline για να μην υπάρχουν πολλαπλοί ορισμοί τους
inline int count_cars = 0;
inline int count_bikes = 0;
inline int count_st_v = 0;
inline int count_traf_signs = 0;
inline int count_traf_lights = 0;

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
        WorldObject(const char& glyph, const string& type, float x, float y)
        : GLYPH(glyph), TYPE(type)
        {
            POSITION.x = x;
            POSITION.y = y;  
        }
        //Destructor
        virtual ~WorldObject() {}
        
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
        //Constructor
        MovingObjects(const char& glyph, const string& type, float x, float y, 
        const string& speed, Direction direction, const string& obj_type)
        : WorldObject(glyph, type, x, y), SPEED(speed), DIRECTION(direction), object_type(obj_type) 
        {
            cout << "Created at (" << POSITION.x << ")" << "," <<"(" << POSITION.y << ")" 
            << " heading (" << DIRECTION.x << ")" << "," << "("<< DIRECTION.y <<")"
            << " at (" << SPEED << ") units/tick" << endl;
        }

        //Destructor
        virtual ~MovingObjects() {}

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
        virtual string getSpeed() const = 0;
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
            count_cars++;
            ID = object_type.append(to_string(count_cars));
            cout << "Object has id: " << ID << endl;
        }
        //Destructor
        ~CARS() override 
        { 
            count_cars--;
            //cout << "Being scrapped..." << endl;
        }
        
        //Getter function for speed
        string getSpeed() const override { return SPEED; }
};

class BIKES : public MovingObjects
{
    public:
        //Constructor
        BIKES(const char& glyph, const string& type, float x, float y, 
        const string& speed, Direction direction, const string& obj_type)
        : MovingObjects(glyph, type, x, y, speed, direction, obj_type)
        {
            count_bikes++;
            ID = object_type.append(to_string(count_bikes));
            cout << "Object has id: " << ID << endl;
        }
        //Destructor
        ~BIKES() override 
        { 
            count_bikes--; 
            //cout << "Being locked away..." << endl;
        }

        //Getter function for speed
        string getSpeed() const override { return SPEED; }
};

//Generative class 2 of Objects//
class StaticObjects : public WorldObject
{
    protected:
        string object_type_s;
    public:
        //Constructor
        StaticObjects(const char& glyph, const string& type, float x, float y, const string& obj_type_s)
        : WorldObject(glyph, type, x, y), object_type_s(obj_type_s) {}
        
        //Destructor
        virtual ~StaticObjects() {}  
};

//GENERATIVE CLASSES OF STATIC OBJECTS//

class STAT_VEH : public StaticObjects
{
    public:
        //Constructor
        STAT_VEH(const char& glyph, const string& type, float x, float y, const string& obj_type_s)
        : StaticObjects(glyph, type, x, y, obj_type_s)
        {
            ID = object_type_s.append(to_string(count_st_v));
            cout << ID << " is parked at (" << POSITION.x << ")" << "," <<" (" << POSITION.y <<")" << endl;
            count_st_v++;
            //cout << "Object id: " << ID << endl;
        } 
        //Destructor
        ~STAT_VEH() override 
        { 
            count_st_v--;
            //cout << "I'm being towed away!" << endl;
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
        : StaticObjects(glyph, type, x, y, obj_type_s), ID_TEXT(id_text)
        {
            count_traf_signs++;
            ID = object_type_s.append(to_string(count_traf_signs));
            //cout << " Object id: " << ID << endl; 
        }
        //Destructor
        ~TRAFFIC_SIGNS() override { count_traf_signs--; }
        
        //Getter function
        string getSignText() const { return ID_TEXT; }
};

class TRAFFIC_LIGHTS : public StaticObjects
{
    private:
        string COLOUR;
    public:
        //Constructor
        TRAFFIC_LIGHTS(const char& glyph, const string& type, float x, float y, const string& obj_type_s, const string& colour)
        : StaticObjects(glyph, type, x, y, obj_type_s), COLOUR(colour)
        {
            count_traf_lights++;
            ID = object_type_s.append(to_string(count_traf_lights));
            cout << ID << " is initialized at ("<< POSITION.x << ")" << "," << "("<< POSITION.y <<")" 
            << " to ("<< COLOUR <<")" << endl;
            //cout << "Object id: " << ID << endl; 
        }
        //Destructor
        ~TRAFFIC_LIGHTS() override 
        { 
            count_traf_lights--; 
            //cout << "Turning off" << endl;
        }

        // Update light colour based on tick (RED 4, GREEN 8, YELLOW 2)
        void updateTick(unsigned int tick, unsigned int /*dimX*/, unsigned int /*dimY*/) override
        {
            unsigned int cycle = tick % 14u;
            if (cycle < 4u) COLOUR = "RED";
            else if (cycle < 12u) COLOUR = "GREEN";
            else COLOUR = "YELLOW";
        }

        //Getter function
        string getColour() const { return COLOUR; }
};

#endif // WORLD_H