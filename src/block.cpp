#include "block.h"

Block::Block(float X, float Y, const char* P, uint8_t t) : 
    Object2d(X, Y, P), type(t)
{ }

Block::Block(float X, float Y, uint8_t t, uint8_t l, const char* P) : 
    Object2d(X, Y, P), type(t), lane(l)
{ }

// Setters
void Block::setOnScreen(bool os) { on_screen = os; }
void Block::setHasSpriteIndex(bool hsi) { has_sprite_index = hsi; }
void Block::setSpriteIndex(uint16_t si) { sprite_index = si; }
void Block::setCanActivate(bool ca) { can_activate = ca; }
void Block::setReverse(bool r) { reverse = r; }
void Block::setStoodOn(bool so) { stood_on = so; }
void Block::setClimbedOn(bool co) { climbed_on = co; }
void Block::setTravelDistGridX(int16_t tdgx) { dist_grid_x_max = tdgx; }
void Block::setTravelDistGridY(int16_t tdgy) { dist_grid_y_max = tdgy; }

// Getters
uint16_t Block::getInitGridX() { return init_grid_x; }
uint16_t Block::getInitGridY() { return init_grid_y; }
bool Block::getOnScreen() { return on_screen; }
bool Block::getManual() { return manual; }
uint8_t Block::getType() { return type; }
bool Block::getHasSpriteIndex() { return has_sprite_index; }
uint16_t Block::getSpriteIndex() { return sprite_index; }
uint8_t Block::getLane() { return lane; }
bool Block::getCanActivate() { return can_activate; }
bool Block::getReverse() { return reverse; }
bool Block::getStoodOn() { return stood_on; }
bool Block::getClimbedOn() { return climbed_on; }
int16_t Block::getTravelDistGridX() { return dist_x_max / getGrid(); }
int16_t Block::getTravelDistGridY() { return dist_y_max / getGrid(); }

void Block::init(float sf)
{
    if (first_init)
    {
        init_grid_x = getX();
        init_grid_y = getY();
        first_init = false;
    }
    updateScale(sf);
    setX(init_grid_x * getGrid());
    setY(init_grid_y * getGrid());
    dist_x_max = dist_grid_x_max * getGrid();
    dist_y_max = dist_grid_y_max * getGrid();
}

void Block::initMove(bool m)
{
    manual = m;
    setAccelX(getGrid() * 2);
    setAccelY(getGrid() * 2);

    dist_x = 0;
    dist_y = 0;
}

void Block::halfAccel(float dt)
{
    // If block has to travel in the x-axis
    if (dist_x_max)
    {
        // If distance travelled is below 2 grid, accel
        if (abs(dist_x) < getGrid() * 2) 
            setVelX(getVelX() + dir_x * getAccelX() * dt * 0.5f);
        // If distance left is below 2 grid, decel
        else if (abs(dist_x) > abs(dist_x_max) - getGrid() * 2)
            setVelX(getVelX() - dir_x * getAccelX() * dt * 0.5f);
    }
    // If block has to travel in the y-axis
    if (dist_y_max)
    {
        if (abs(dist_y) < getGrid() * 2)
            setVelY(getVelY() + dir_y * getAccelY() * dt * 0.5f);
        else if (abs(dist_y) > abs(dist_y_max) - getGrid() * 2)
            setVelY(getVelY() - dir_y * getAccelY() * dt * 0.5f);
    }
}

void Block::move(float dt)
{
    dir_x = (reverse ? -1 : 1) * (dist_x_max > 0 ? 1 : -1);
    dir_y = (reverse ? -1 : 1) * (dist_y_max > 0 ? 1 : -1);

    // First half accel
    halfAccel(dt);

    // Movement
    setX(getX() + getVelX() * dt);
    dist_x += abs(getVelX()) * dt;
    setY(getY() + getVelY() * dt);
    dist_y += abs(getVelY()) * dt;

    // Second half accel
    halfAccel(dt);

    // Reset speed, distance and reverse the travel direction
    if (abs(dist_x) >= abs(dist_x_max) && abs(dist_y) >= abs(dist_y_max))
    {
        setVelX(0);
        setVelY(0);
        dist_x = 0;
        dist_y = 0;
        reverse = !reverse;
    }
}

void Block::reset()
{
    setVelX(0);
    setVelY(0);

    dist_x = 0;
    dist_y = 0;

    reverse = false;

    setX(init_grid_x * getGrid());
    setY(init_grid_y * getGrid());
}

Block::~Block()
{
    // std::cout << "Block terminated!\n";
}