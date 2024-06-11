#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <math.h>

#include "object2d.h"
#include "input.h"

class Player : public Object2d
{
private:
    bool on_ground = false;

    bool right = true;  // true: facing right
                        // false: facing left

    float vel_terminal = -700;
    float vel_x_max = 500;

    // No jitter decel
    float temp_speed_left;
    float temp_speed_right;
public:
    Player(float X = 0, float Y = 0, const char* P = "");
    
    void setOnGround(bool og);
    bool getOnGround();

    void playerInit();
    void playerMovement(Input *input, float dt);
};

#endif