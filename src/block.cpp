#include "block.h"

Block::Block(float X, float Y, const char* P, uint8_t t) : Object2d
    (X, Y, P), type(t)
{ }