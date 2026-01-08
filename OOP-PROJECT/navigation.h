#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <vector>
#include <optional>
#include <random>
#include "types.h"
#include "sensor_reading.h"
#include "fusion.h"

using namespace std;
static unsigned int navIdCounter = 0;
// The NavigationSystem class controls vehicle navigation by:
// Managing a list of GPS target positions (waypoints).
//Fusing sensor data through a SensorFusionEngine.
// Making driving decisions (direction and speed adjustment)
// based on sensor readings and current navigation progress.
class NavigationSystem {
    private:
    // List of target positions (the route or waypoints)
        vector<Position> targets;
        // Index of the current target in the list.
        size_t current = 0;
        // Stores the most recent fused sensor readings.
        vector<SensorReading> lastFused;
        // The fusion engine used to combine raw sensor inputs.
        SensorFusionEngine fusionEngine;
        unsigned int id;
        string speed;
        Direction carDirection;
    public:
        //Constructor
        NavigationSystem() : fusionEngine(40), id(++navIdCounter), speed("STOPPED"), carDirection({0,1})  
        {
           cout << "[+NAV:" << id << "] Hello, I'll be your GPS today" << endl;
        }
        //Constructor
        NavigationSystem(unsigned int minConf) : fusionEngine(minConf), id(++navIdCounter), speed("STOPPED"), carDirection({0,1}) 
        {
            cout << "Hello, I'll be your GPS today" << endl;
        }
        //Destructor
        ~NavigationSystem() {}

        // Set the navigation route by specifying target positions.
        // Resets the current target index to the beginning.
        void setTargets(const vector<Position>& t) { targets = t; current = 0; }
        optional<Position> getCurrentTarget() const 
        {
            // Returns the current navigation target (if available).
            if (current < targets.size()) return targets[current];
            return {};
        }

        //Gets fused sensor readings
        void receiveFusedReadings(const vector<SensorReading>& fused) { lastFused = fused; }

        //Set the internal state of the self driving car
        void setCarState(const string& spd, const Direction& dir) 
        {
            speed = spd;
            carDirection = dir;
        }

        // Fuses raw sensor data using the fusion engine and stores the result.
        // Returns the fused readings for optional further processing
        vector<SensorReading> fuseSensorData(const vector<SensorReading>& raw) 
        {
            auto fused = fusionEngine.fuseSensorData(raw);
            receiveFusedReadings(fused);
            return fused;
        }

        // Core decision-making function:
        // Determines the next movement direction and recommended action (ACCELERATE/DECELERATE/MAINTAIN)
        // based on the current position, target, and sensor observations.
        pair<Direction, string> makeDecisionWithFused(const Position& myPos, const vector<SensorReading>& fusedData) 
        {
            Direction moveDir{0,0};
            string action = "MAINTAIN";
            //If no gps target, car doesn't move and doesn't change it's speed
            auto tgtOpt = getCurrentTarget();
            if (!tgtOpt.has_value()) return {moveDir, action};
            Position tgt = *tgtOpt;
            
            // Calculate direction to target
            // Priority: move vertically until y is aligned, then move horizontally
            int dx = tgt.x - myPos.x;
            int dy = tgt.y - myPos.y;
            
            // Move vertically first (NORTH or SOUTH) to align y-coordinate
            // Only move horizontally after y is aligned
            if (dy != 0) {
                moveDir.y = (dy > 0 ? 1 : -1);
                moveDir.x = 0;
            } else if (dx != 0) {
                moveDir.x = (dx > 0 ? 1 : -1);
                moveDir.y = 0;
            }

            // Check for deceleration conditions
            bool decelerate = false;
            
            // Check traffic lights (RED or YELLOW within 3 cells)
            for (const auto& r : fusedData) 
            {
                if (!r.lightColour.empty() && (r.lightColour == "RED" || r.lightColour == "YELLOW") && 
                    r.distance >= 0 && r.distance <= 3) 
                {
                    decelerate = true;
                    cout << "  [SENSOR] Traffic light " << r.lightColour << " at distance " << r.distance << " -> DECELERATE" << endl;
                    break;
                }
            }
            
            // Check obstacles (CAR or BIKE within 2 cells)
            if (!decelerate) 
            {
                for (const auto& r : fusedData) 
                {
                    if ((r.type == "CAR" || r.type == "BIKE") && r.distance >= 0 && r.distance <= 2) 
                    {
                        decelerate = true;
                        cout << "  [SENSOR] Obstacle " << r.type << " at distance " << r.distance << " -> DECELERATE" << endl;
                        break;
                    }
                }
            }
            
            // Check if approaching GPS target - decelerate when AT the target (dist=0)
            if (!decelerate) 
            {
                int distToTarget = abs(dx) + abs(dy);
                if (distToTarget == 0) 
                {
                    decelerate = true;
                }
            }

            if (decelerate) action = "DECELERATE";
            else action = "ACCELERATE";

            return {moveDir, action};
        }

        // Move to next target if arrived (use exact coordinate matching)
        void checkArrival(const Position& myPos) 
        {
            if (current >= targets.size()) return;
            Position tgt = targets[current];
            // Only advance to next target if we've EXACTLY reached the target coordinates
            // This prevents premature advancement when doing multi-step movements
            if (myPos.x == tgt.x && myPos.y == tgt.y) 
            {
                ++current;
            }
        }

        // Check arrival BEFORE decision making (so new target is used for next decision)
        void checkArrivalBeforeTick(const Position& myPos) 
        {
            if (current >= targets.size()) return;
            Position tgt = targets[current];
            if (myPos.x == tgt.x && myPos.y == tgt.y) 
            {
                ++current;
            }
        }
};

#endif // NAVIGATION_H