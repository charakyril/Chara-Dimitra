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

//MAKE STRUCT POSITION
struct Position
{
    int x;
    int y;
};

//Struct for moving object car
struct Cars 
{
    int count;
};
//struct for moving object bike
struct Bikes
{
    int count;
};
//struct for static object stationary vehicle
struct StatVehicle
{
    int count;
};
//struct for static object traffic signs
struct TrafficSigns
{
    string ID_TEXT;
    int count;
};
//struct for static object traffic lights
struct TrafficLights
{
    string COLOUR;
    int count;
};
//----STRUCTS FOR SENSORS----//
//Lidar detector
struct LidarDetector
{
    float distance;
    string object_type_detect;
    float sureness;
};

//Radar detector
struct RadarDetector
{
    float distance;
    float speed;
    Direction movement_direction;
    float sureness;
};

//Camera sensor
struct CameraSensor
{
    string object_type_detect;
    float distance;
    //struct position
    Position position;
    string ObjectID;
    float sureness;
    float speed;
    Direction movement_direction;
    string SignText;
    //GREEN, YELLOW, RED    
    string LightColour;
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
        //I will make a struct Direction
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
        Cars car;
    public:
        //Constructor
        CARS(const char& glyph, const string& type, float x, float y,
         int speed, Direction direction, const string& obj_type, Cars car)
        : MovingObjects(glyph, type, x, y, speed, direction, obj_type), car(car)
        {
            car.count++;
            ID = object_type.append(to_string(car.count));
            cout << "I just made a moving object of type: " << object_type << endl;
        }
        //Destructor
        ~CARS() override
        {
            car.count--;
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
        Bikes bike;
    public:
        //Constructor
        BIKES(const char& glyph, const string& type, float x, float y, int speed, Direction direction, const string& obj_type, Bikes bike)
        : MovingObjects(glyph, type, x, y, speed, direction, obj_type), bike(bike)
        {
            bike.count++;
            ID = object_type.append(to_string(bike.count));
            cout << "I just made a moving object of type: " << object_type << endl;
        }
        //Destructor
        ~BIKES() override
        {
            bike.count--;
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
        StatVehicle statveh;
    public:
        //Constructor
        STAT_VEH(const char& glyph, const string& type, float x, float y, const string& obj_type_s, StatVehicle statveh)
        : StaticObjects(glyph, type, x, y, obj_type_s), statveh(statveh)
        {
            statveh.count++;
            ID = object_type_s.append(to_string(statveh.count));
            cout << "I just made a static object of type: " << object_type_s << endl;
        } 
        ~STAT_VEH() override
        {
            statveh.count--;
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
        TrafficSigns traf_signs;
    public: 
        //Constructor
        TRAFFIC_SIGNS(const char& glyph, const string& type, float x, float y, const string& obj_type_s, TrafficSigns traf_signs)
        : StaticObjects(glyph, type, x, y, obj_type_s), traf_signs(traf_signs)
        {
            traf_signs.count++;
            ID = object_type_s.append(to_string(traf_signs.count));
            cout << "I just made a static object of type: " << object_type_s << endl;
        }
        //Destructor
        ~TRAFFIC_SIGNS() override
        {
            traf_signs.count--;
            cout << "I just destroyed a static object of type: " << object_type_s << endl;
        }
        void describe() const override
        {
            cout << "Static object is of type: " << object_type_s << endl;
            cout << "This traffic sing has id: " << ID << endl;
        }
};

class TRAFFIC_LIGHTS : public StaticObjects
{
    private:
        TrafficLights traf_lights;
    public:
        //Constructor
        TRAFFIC_LIGHTS(const char& glyph, const string& type, float x, float y, const string& obj_type_s, TrafficLights traf_lights)
        : StaticObjects(glyph, type, x, y, obj_type_s), traf_lights(traf_lights)
        {
            traf_lights.count++;
            ID = object_type_s.append(to_string(traf_lights.count));
            cout << "I just made a static object of type: " << object_type_s << endl;
        }
        //Destructor
        ~TRAFFIC_LIGHTS() override
        {
            traf_lights.count--;
            cout << "I just destroyed a static object of type: " << object_type_s << endl;
        }
        //Describe
        void describe() const override
        {
            cout << "Static object is of type: " << object_type_s << endl;   
            cout << "This traffic light has id: " << ID << "and has colour: " << traf_lights.COLOUR << endl;
        }
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
        LidarDetector lidar_detector;
        //To find distance
        Position pos;
        float distance;
        string obj_type;
        //Bebaiothta
        float sure;
    public:
        //Constructor
        Lidar(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy,
        LidarDetector lidar_detector, Position pos, const string& obj_type, float sure)
        : Sensors(sens_type, range, vis_range, accuracy), lidar_detector(lidar_detector), pos(pos), obj_type(obj_type), sure(sure)
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
        RadarDetector radar_detector;
        //To find distance
        Position pos;
        float distance;
        float speed;
        Direction movement_direction;
        float sure;
    public:
        //Constructor
        Radar(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy,
        RadarDetector radar_detector, Position pos, float speed, Direction movement_direction, float sure)
        : Sensors(sens_type, range, vis_range, accuracy), radar_detector(radar_detector), pos(pos),
        speed(speed), movement_direction(movement_direction), sure(sure)
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
        Position pos;
        string Objectid;
        float sure;
        float speed;
        Direction movement_direction;
        string signtext;    
        string lightcolour;
    public: 
        //Constructor
        Camera(const string& sens_type, unsigned int range, unsigned int vis_range, int accuracy,
        const string& object_type_detect, Position pos, const string& Objectid, float sure,
        float speed, Direction movement_direction, const string& signtext, const string& lightcolour)
        : Sensors(sens_type, range, vis_range, accuracy), object_type_detect(object_type_detect), pos(pos),
        Objectid(Objectid), sure(sure), speed(speed), movement_direction(movement_direction), signtext(signtext), lightcolour(lightcolour)
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
        Direction direction;
        direction.x = stof(argv[1]);
        direction.y = stof(argv[2]);
        cout << "I took coordinates: " << direction.x << " " << direction.y << endl;
    }

    return 0;
}