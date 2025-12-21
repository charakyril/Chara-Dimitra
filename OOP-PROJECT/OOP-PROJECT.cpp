#include <iostream>
#include <string>
#include <cctype>

using namespace std;

//Global variable ticks//
unsigned int ticks = 0;

//-----STRUCTS-----//

//MAKE STRUCT DIRECTION FOR OBJECT
struct Direction 
{
    //Pairs of coordinates
    float x;
    float y;
};

//STRUCT POSITION for gps and coordinates
struct Position
{
    int x;
    int y;
};

//-------CLASSES-------//

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
        int SPEED;
        Direction DIRECTION;
        //Car or bicycle
        string object_type;
    public:
        //Constructor
        MovingObjects(const char& glyph, const string& type, float x, float y, int speed, Direction direction, const string& obj_type)
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
         int speed, Direction direction, const string& obj_type, int counter)
        : MovingObjects(glyph, type, x, y, speed, direction, obj_type), count(counter)
        {
            car.count++;
            ID = object_type.append(to_string(car.count));
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
        BIKES(const char& glyph, const string& type, float x, float y, int speed, Direction direction, const string& obj_type, int counter)
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

//----CLASS FOR SelfDrivingCar----//
class SelfDrivingCar
{
    protected:
        Direction car_direction;
        //SPEED = STOPPED, HALF_SPEED, FULL_SPEED
        string speed;
        //ALL SENSORS
        LidarDetector first_sensor;
        RadarDetector second_sensor;
        CameraSensor third_sensor;
        //class navigation system
        NavigationSystem nav_system;
    public:
        //Constructor
        //Destructor
        //Describe
};

//Class for NavigationSystem
class NavigationSystem
{
    private:
        //GPS targets
        float gps_x;
        float gps_y;
    public:
        //Constructor
        //Destructor
        //fuseSensorData with SensorFusionEngine and MakeDesicion to move
};

//-----SENSORS-----//
class Sensors 
{
    protected:
        string SENS_TYPE;
        unsigned int RANGE;
        unsigned int VIS_RANGE;
        int ACCURACY;
    public:
        //Constructor
        Sensors(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy)
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
    private:
        float distance;
        string object_type_detect;
        float sureness;
    public:
        //Constructor
        Lidar(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy,
        float dist, const string& detect_object, float sure)
        : Sensors(sens_type, range, vis_range, accuracy), distance(dist), object_type_detect(detect_object), sureness(sure)
        {
            cout << "One working lidar sensor\n";
            //Find distance
        }
        //Destructor
        ~Lidar() override
        {
            cout << "Lidar sensor stopped operating\n";
        }
        void describe_sensor() const override
        {
            cout << "Sensor is of type: " << SENS_TYPE << endl;
        }
};

//----SENSOR RADAR----//
class Radar : public Sensors
{
    private:
        float distance;
        //SPEED = STOPPED, HALF_SPEED, FULL_SPEED
        string speed;
        Direction movement_direction;
        float sureness;
    public:
        //Constructor
        Radar(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy,
        float dist, const string& sp, Direction dir, float sure)
        : Sensors(sens_type, range, vis_range, accuracy), distance(dist), speed(sp), movement_direction(dir), sureness(sure)
        {
            cout << "One working radar sensor\n";
            //Find distance
        }
        //Destructor
        ~Radar() override
        {
            cout << "Radar sensor stopped operating\n";
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
    private:
        string object_type_detect;
        float distance;
        //struct position
        Position position;
        string ObjectID;
        float sureness;
        //SPEED = STOPPED, HALF_SPEED, FULL_SPEED
        string speed;
        Direction movement_direction;
        //Signs can be STOP, YIELD etc
        string SignText;
        //GREEN, YELLOW, RED    
        string LightColour;
    public: 
        //Constructor
        Camera(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy,
        const string& detect_object, float dist, Position pos, const string& obj_id, float sure, const string& sp,
        Direction dir, const string& signtext, const string& lightcolour)
        : Sensors(sens_type, range, vis_range, accuracy), object_type_detect(detect_object), distance(dist), position(pos),
        ObjectID(obj_id), sureness(sure), speed(sp), movement_direction(dir), SignText(signtext), LightColour(lightcolour)
        {
            cout << "One camera sensor working\n";
            //Find distance
        }       
        //Destructor
        ~Camera() override
        {
            cout << "Camera sensor stopped working\n";
        }
        //Describe
        void describe_sensor() const override
        {
            cout << "Sensor is of type: " << SENS_TYPE << endl;
        }
};

//GPS targets as arguments from user
int main(int argc, char* argv[])
{

    if(argc < 3 || argc > 3)
    {
        cout << "Right usage: <executable progam>, <X1>, <Y1>,..." << endl;
    }
    if(argc == 3)
    {
        Position start_pos;
        start_pos.x = stof(argv[1]);
        start_pos.y = stof(argv[2]);
        cout << "I took coordinates: " << start_pos.x << " " << start_pos.y << endl;
    }

    return 0;
}
