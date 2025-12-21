#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <string>
#include "types.h"
using namespace std;


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
            cout << "I just made an object\n";
        }
        //Destructor
        virtual ~WorldObject()
        {
            cout << "I just destroyed an object\n";
        }
        //Virtual describe function for polymorphism
        virtual void describe() const = 0;
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
        MovingObjects(const char& glyph, const string& type, float x, float y, const string& speed, Direction direction, const string& obj_type)
        : WorldObject(glyph, type, x, y), SPEED(speed), DIRECTION(direction), object_type(obj_type)
        {
            cout << "I just made a moving object of type: " << object_type << endl;
        }
        virtual ~MovingObjects()
        {
            cout << "I just destroyed a moving object of type: " << object_type << endl;
        }
        //Describe function
        virtual void describe() const = 0;
};

//GENERATIVE CLASSES FOR MOVING OBJECTS//

class CARS : public MovingObjects
{
    private:
        int count;
    public:
        //Constructor
        CARS(const char& glyph, const string& type, float x, float y,
        const string& speed, Direction direction, const string& obj_type, int counter)
        : MovingObjects(glyph, type, x, y, speed, direction, obj_type), count(counter)
        {
            count++;
            ID = object_type.append(to_string(count));
            cout << "I just made a moving object of type: " << object_type << endl;
        }
        //Destructor
        ~CARS() override
        {
            count--;
            cout << "I just destroyed a moving object of type: " << object_type << endl;
        }
        void describe() const override
        {
            cout << "Moving object is of type: " << object_type << "and has speed: " << SPEED << endl;
            cout << "This car has id: " << ID << endl;
        }
};

class BIKES : public MovingObjects
{
    private:
        int count;
    public:
        //Constructor
        BIKES(const char& glyph, const string& type, float x, float y, const string& speed, Direction direction, const string& obj_type, int counter)
        : MovingObjects(glyph, type, x, y, speed, direction, obj_type), count(counter)
        {
            count++;
            ID = object_type.append(to_string(count));
            cout << "I just made a moving object of type: " << object_type << endl;
        }
        //Destructor
        ~BIKES() override
        {
            count--;
            cout << "I just destroyed a moving object of type: " << object_type << endl;
        }
        void describe() const override
        {
            cout << "Moving object is of type: " << object_type << "and has speed: " << SPEED << endl;
            cout << "This bike has id: " << ID << endl;
        }
};

//Generative class 2 of Objects//
class StaticObjects : public WorldObject
{
    protected:
        string object_type_s;
    public:
        StaticObjects(const char& glyph, const string& type, float x, float y, const string& obj_type_s)
        : WorldObject(glyph, type, x, y), object_type_s(obj_type_s)
        {
            cout << "I just made a static object of type: " << object_type_s << endl;
        }
        virtual ~StaticObjects()
        {
            cout << "I just destroyed a static object of type: " << object_type_s <<endl;
        }
        //Describe function
        virtual void describe() const = 0;
    
};
//GENERATIVE CLASSES OF STATIC OBJECTS//

class STAT_VEH : public StaticObjects
{
    private:
        int count;
    public:
        //Constructor
        STAT_VEH(const char& glyph, const string& type, float x, float y, const string& obj_type_s, int counter)
        : StaticObjects(glyph, type, x, y, obj_type_s), count(counter)
        {
            count++;
            ID = object_type_s.append(to_string(count));
            cout << "I just made a static object of type: " << object_type_s << endl;
        } 
        ~STAT_VEH() override
        {
            count--;
            cout << "I just destroyed a static object of type: " << object_type_s << endl;
        }
        void describe() const override
        {
            cout << "Static object is of type: " << object_type_s << "and is at position: " << POSITION.x << " " << POSITION.y << endl;
            cout << "This static vehicle has id: " << ID << endl;
        } 
};

class TRAFFIC_SIGNS : public StaticObjects
{
    private: 
        string ID_TEXT;
        int count;
    public: 
        //Constructor
        TRAFFIC_SIGNS(const char& glyph, const string& type, float x, float y, const string& obj_type_s, 
        const string& id_text, int counter)
        : StaticObjects(glyph, type, x, y, obj_type_s), ID_TEXT(id_text), count(counter)
        {
            count++;
            ID = object_type_s.append(to_string(count));
            cout << "I just made a static object of type: " << object_type_s << endl;
        }
        //Destructor
        ~TRAFFIC_SIGNS() override
        {
            count--;
            cout << "I just destroyed a static object of type: " << object_type_s << endl;
        }
        void describe() const override
        {
            cout << "Static object is of type: " << object_type_s << endl;
            cout << "This traffic sing has id: " << ID << "and is a " << ID_TEXT << " sign" <<  endl;
        }
};

class TRAFFIC_LIGHTS : public StaticObjects
{
    private:
        string COLOUR;
        int count;
    public:
        //Constructor
        TRAFFIC_LIGHTS(const char& glyph, const string& type, float x, float y, const string& obj_type_s, 
        const string& colour, int counter)
        : StaticObjects(glyph, type, x, y, obj_type_s), COLOUR(colour), count(counter)
        {
            count++;
            ID = object_type_s.append(to_string(count));
            cout << "I just made a static object of type: " << object_type_s << endl;
        }
        //Destructor
        ~TRAFFIC_LIGHTS() override
        {
            count--;
            cout << "I just destroyed a static object of type: " << object_type_s << endl;
        }
        //Describe
        void describe() const override
        {
            cout << "Static object is of type: " << object_type_s << endl;   
            cout << "This traffic light has id: " << ID << "and has colour: " << COLOUR << endl;
        }
};

#endif // WORLD_H