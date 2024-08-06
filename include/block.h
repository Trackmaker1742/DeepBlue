#ifndef BLOCK_H
#define BLOCK_H

#include <SDL2/SDL.h>
#include <vector>

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

                    // 6: Square block
                    // 7: Slope left
                    // 8: Slope right
                    // 9: Wall (both sides)
                    // 10: Moving block

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

    uint8_t lane;   // For rhythm game
public:
    Block(float X = 0, float Y = 0, const char* P = "", uint8_t t = 0);
    Block(float X = 0, float Y = 0, uint8_t t = 0, uint8_t l = 0, const char* P = ""); 

    uint8_t getType();
    uint8_t getLane();
};

#endif 