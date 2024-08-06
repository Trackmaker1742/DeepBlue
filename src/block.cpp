#include "block.h"

Block::Block(float X, float Y, const char* P, uint8_t t) : Object2d
    (X, Y, P), type(t)
{ }

Block::Block(float X, float Y, uint8_t t, uint8_t l, const char* P) : Object2d
    (X, Y, P), type(t), lane(l)
{ }

uint8_t Block::getType() { return type; }
uint8_t Block::getLane() { return lane; }