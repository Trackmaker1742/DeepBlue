#include "block.h"

Block::Block(float X, float Y, const char* P, uint8_t t) : 
    Object2d(X * 64, Y * 64, P), type(t)
{ }

Block::Block(float X, float Y, uint8_t t, uint8_t l, const char* P) : 
    Object2d(X * 64, Y * 64, P), type(t), lane(l)
{ }

void Block::initMove(bool m)
{
    manual = m;
    setAccelX(getGrid() * 2);
    setAccelY(getGrid() * 2);
}

void Block::move(float dt)
{
    short direction = reverse ? -1 : 1;

    // First half accel
    if (dist_x_max)
    {
        if (dist_x < getGrid() * 2) 
            setVelX(getVelX() + getAccelX() * direction * dt * 0.5f);
        else if (dist_x > dist_x_max - getGrid() * 2)
            setVelX(getVelX() - getAccelX() * direction * dt * 0.5f);
    }

    if (dist_y_max)
    {
        if (dist_y < getGrid() * 2) 
            setVelY(getVelY() + getAccelY() * direction * dt * 0.5f);
        else if (dist_y > dist_y_max - getGrid() * 2)
            setVelY(getVelY() - getAccelY() * direction * dt * 0.5f);
    }

    // Movement
    setX(getX() + getVelX() * dt);
    dist_x += abs(getVelX()) * dt;
    setY(getY() + getVelY() * dt);
    dist_y += abs(getVelY()) * dt;

    // Second half accel
    if (dist_x_max)
    {
        if (dist_x < getGrid() * 2) 
            setVelX(getVelX() + getAccelX() * direction * dt * 0.5f);
        else if (dist_x > dist_x_max - getGrid() * 2)
            setVelX(getVelX() - getAccelX() * direction * dt * 0.5f);
    }

    if (dist_y_max)
    {
        if (dist_y < getGrid() * 2) 
            setVelY(getVelY() + getAccelY() * direction * dt * 0.5f);
        else if (dist_y > dist_y_max - getGrid() * 2)
            setVelY(getVelY() - getAccelY() * direction * dt * 0.5f);
    }

    if (dist_x >= dist_x_max && dist_y >= dist_y_max)
    {
        setVelX(0);
        setVelY(0);
        dist_x = 0;
        dist_y = 0;
        reverse = !reverse;
    }
}

// Setters
void Block::setCanActivate(bool ca) { can_activate = ca; }
void Block::setReverse(bool r) { reverse = r; }
void Block::setStoodOn(bool so) { stood_on = so; }
void Block::setClimbedOn(bool co) { climbed_on = co; }
void Block::setTravelDistX(uint16_t tdx) { dist_x_max = tdx; };
void Block::setTravelDistY(uint16_t tdy) { dist_y_max = tdy; };

// Getters
uint8_t Block::getType() { return type; }
uint8_t Block::getLane() { return lane; }
bool Block::getCanActivate() { return can_activate; }
bool Block::getReverse() { return reverse; }
bool Block::getStoodOn() { return stood_on; }
bool Block::getClimbedOn() { return climbed_on; }

Block::~Block()
{
    std::cout << "Block terminated!\n";
}