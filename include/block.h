#ifndef BLOCK_H
#define BLOCK_H

#include <SDL2/SDL.h>
#include <vector>
#include <math.h>

#include "object2d.h"

// Due to some coding issue, 
// block's x and y coordinates are obtained through gridX and gridY
// instead of just getX and getY

class Block : public Object2d
{
private:
    uint8_t type;   // 0: Emptyness (csv)
                    // 1: Spawn block
                    // 2: Checkpoint block
                    // 3: Goal block
                    // 4: Danger block
                    // 5: Item block (24 items planned, probably will be more if possible)

                    // 6: Square block (9 sprites)
                    
                    // 7: Slope up left     // Can't move properly
                    // 8: Slope up right    // Can't move properly
                    // 9: Slope down left   // Should be able to move
                    // 10: Slope down right // Should be able to move
                    
                    // 11: Wall (both sides) (3 sprites)
                    // 12: Bridge (player can jump through it from 
                    //              the bottom, bottom left, right side)
                    
                    // 13: Spring (up)
                    // 14: Spring (down)
                    // 15: Spring (left)
                    // 16: Spring (right)

                    // New moving block formatting style
                    // Give each block a certain amount of moves to each direction
                    // For example: 06|13|10|1 would be to move a normal block (type 6)
                    // 13 grid in the x axis, 10 grid in the y axis
                    // 1 will be a manual moving block (require player to stand on to start moving)
                    // Since it's a boolean, 0 means it is automatically moving block

                    // The plan is to have block with 1 type and multiple sprites to be separated into ranges, 
                    // so that the stage can parse it correctly
                    // Example: grass (6), ice (7), dirt (8) sprites but all uses type 6
                    // Stage will parse 6, 7, 8 as type 6 all at once,
                    // That's the gist of it
                    // That would require me to plan out ahead by quite a bit though
                    // Still, that's the best solution for the time being

                    // So, rhythm game clock does have a few types
                    // 0: Empty void
                    // 1: Spawn
                    // 2: Low geyser
                    // 3: High geyser
                    // 4: Break-able object

    // Initial position of moving block
    uint16_t init_grid_x;
    uint16_t init_grid_y;

    // Travel distance of each block 
    float dist_x;   // Temp dist value for accel and decel
    float dist_y;
    int16_t dist_x_max = 0;   // Max travel distance of each block
    int16_t dist_y_max = 0;

    short dir_x;
    short dir_y;

    // Checkers
    bool on_screen = true;      // To skip collision check when not on screen
    bool manual;                // Manual or auto
    bool can_activate = false;  // To activate manual moving blocks and start their movement
    bool reverse = false;       // To move block back to starting point, completing the move
    bool stood_on = false;
    bool climbed_on = false;

    uint8_t lane;   // For rhythm game
public:
    // Platformer mode blocks
    Block(float X = 0, float Y = 0, const char *P = "", uint8_t t = 0);
    // Rhythm mode blocks
    Block(float X = 0, float Y = 0, uint8_t t = 0, uint8_t l = 0, const char *P = ""); 

    // Initializer for moving blocks
    void initMove(bool manual);

    void halfAccel(float dt);
    // Move function for moving blocks
    void move(float dt);
    // Reset position for moving blocks
    void reset();

    // Setters
    void setOnScreen(bool os);
    void setCanActivate(bool ca);
    void setReverse(bool r);
    void setStoodOn(bool so);
    void setClimbedOn(bool co);
    void setTravelDistX(int16_t tdx);
    void setTravelDistY(int16_t tdy);

    // Getters
    uint16_t getInitGridX();
    uint16_t getInitGridY();
    bool getOnScreen();
    bool getManual();
    uint8_t getType();
    uint8_t getLane();
    bool getCanActivate();
    bool getReverse();
    bool getStoodOn();
    bool getClimbedOn();
    int16_t getTravelDistGridX();
    int16_t getTravelDistGridY();

    ~Block();
};

#endif 