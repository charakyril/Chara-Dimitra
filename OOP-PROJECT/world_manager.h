// This class models a simple 2D simulation world where objects can exist,
// move, and be visualized. It maintains a grid with coordinates (x, y),
// updates objects over time, and prints the world state for visualization.

#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <algorithm>
#include "world.h"
#include "types.h"

using namespace std;

class World
{
    private:
        unsigned int dimX, dimY;
        unsigned int tickCount = 0;
        vector<WorldObject*> pendingDeletion;  // Objects to delete at end of simulation
    public:
        vector<WorldObject*> objects; // owning managed externally for simplicity

        // Constructor: creates a world grid with specified dimensions (default 40x40).
        World(unsigned int x = 40, unsigned int y = 40) : dimX(x), dimY(y) 
        {
            cout << "Reticulating splines! Hello , world!" << endl;
        }
        
        //Destructor - deletes all remaining objects including pending deletions
        ~World() 
        {
            // First, delete any pending out-of-bounds objects
            for (auto p : pendingDeletion) {
                delete p;
            }
            pendingDeletion.clear();
            
            // Then delete all remaining objects in the world
            for (auto p : objects) {
                delete p;
            }
            objects.clear();
            cout << "Goodbye ,cruel world!" << endl;
        }

        // Add a pending deletion - will be deleted at end of simulation
        void addPendingDeletion(WorldObject* obj) {
            pendingDeletion.push_back(obj);
        }

        //Getter functions for grid world dimentions
        unsigned int getDimX() const { return dimX; }
        unsigned int getDimY() const { return dimY; }

        //Function to add objects in grid world
        void addObject(WorldObject* obj) { objects.push_back(obj); }
        
        // Advances the simulation by one tick (time step).
        // Each object's updateTick() method is called to update its state.
        // Objects that move outside the world boundaries are added to pendingDeletion
        // and will be deleted at the end of the simulation to ensure all destructor
        // messages appear at the end, not interspersed during simulation.
        void updateTick() 
        {
            ++tickCount;
            // update each object
            for (auto p : objects) 
            {
                if (p) p->updateTick(tickCount, dimX, dimY);
            }
            // Mark out-of-bounds objects for deferred deletion
            for (auto it = objects.begin(); it != objects.end(); )
            {
                WorldObject* p = *it;
                if (p == nullptr || p->getPosition().x < 0 || p->getPosition().y < 0)
                {
                    // Move to pendingDeletion - deletion happens at end of simulation
                    addPendingDeletion(p);
                    it = objects.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
        // Returns how many ticks (time steps) have passed since simulation start.
        unsigned int getTickCount() const { return tickCount; }

        // simple placement helper
        bool isCellFree(int x, int y) const {
            for (auto p : objects) if (p->getPosition().x == x && p->getPosition().y == y) return false;
            return true;
        }

        // Prints the full grid of the world to the console.
        // Each cell shows:
        //   '.' for empty cells,
        //   special glyphs for objects,
        //   or a custom car symbol ('@' by default) if car position is provided.
        void printFull(char carGlyph='@', const Position* carPos = nullptr) const {
            for (int y = static_cast<int>(dimY)-1; y >= 0; --y) {
                for (int x = 0; x < static_cast<int>(dimX); ++x) {
                    if (carPos && carPos->x == x && carPos->y == y) { cout << carGlyph; continue; }
                    char out = '.';
                    // Priority scanning: look for special glyphs
                    for (auto p : objects) {
                        if (p->getPosition().x == x && p->getPosition().y == y) {
                            out = p->getGlyph(); break;
                        }
                    }
                    cout << out;
                }
                cout << '\n';
            }
        }

        // Print a square centered around the car (6x6 grid)
        void printAround(const Position& pos, unsigned int radius=3, char carGlyph='@') const
        {
            int minx = max(0, pos.x - static_cast<int>(radius));
            int maxx = min(static_cast<int>(dimX)-1, pos.x + static_cast<int>(radius));
            int miny = max(0, pos.y - static_cast<int>(radius));
            int maxy = min(static_cast<int>(dimY)-1, pos.y + static_cast<int>(radius));

            for (int y = maxy; y >= miny; --y) 
            {
                for (int x = minx; x <= maxx; ++x) 
                {
                    //Print "." for empty cells 
                    if (pos.x == x && pos.y == y) { cout << carGlyph; continue; }
                    char out = '.';
                    //For objects, print each object's glyph
                    for (auto p : objects) 
                    {
                        if (p->getPosition().x == x && p->getPosition().y == y) { out = p->getGlyph(); break; }
                    }
                    cout << out;
                }
                cout << '\n';
            }
        }
};

#endif // WORLD_MANAGER_H