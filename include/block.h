#ifndef BLOCK_H
#define BLOCK_H

#include <SDL2/SDL.h>

#include "object2d.h"

class Block : public Object2d
{
private:
    uint8_t type; // 0: Normal
                // 1: Platform

public:
    Block(float X = 0, float Y = 0, const char* P = "", uint8_t t = 0);
};

#endif 