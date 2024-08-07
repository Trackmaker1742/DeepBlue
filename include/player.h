#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <math.h>

#include "object2d.h"
#include "input.h"
#include "projectile.h"

// Player class can handle all 3 game mode, with different attribute for each

class Player : public Object2d
{
private:
    bool on_ground = false;

    // Dash related variables
    bool can_dash = true;
    bool on_dash = false;
    bool on_dash_delay = false;
    uint8_t dash_frame = 0;
    uint8_t dash_frame_max;     // = fps / 5
    uint8_t dash_frame_delay;   // = fps / 2

    // Wall related variables
    bool on_wall = false;
    float wall_climb_speed;
    bool on_wall_jump = false;
    uint8_t wall_jump_frame = 0;
    uint8_t wall_jump_frame_max;

    // Jump / double jump related variables
    int jump_count = 2;

    bool right = true;  // true: facing right
                        // false: facing left

    float vel_terminal;
    float vel_x_max;

    // No jitter decel
    float temp_speed_left;
    float temp_speed_right;

    // Rhythm stuff
    uint8_t current_lane = 2;   // 1 is the lowest, 3 is the highest
                                //     3
                                //     3
                                //   2
                                //   2                                
                                // 1
                                // 1
                                // Kinda like this
                                // 2 rows because the sprite will be 128x128

    bool rhythm_atk = false;

    // Shooter stuff
    // Will need a different hitbox for both as well
    bool vertical = true;   // To identify each mode during collision calc
    // Vertical
    uint8_t level = 0;      // Increase as more item is collected, max is 4

    // Horizontal
    uint8_t health = 3;     // 3 hp, restart at 0, can be refilled with item
    uint16_t energy = 0;    // Increase as more shot lands, max 300?
    bool dark = true;       // 2 states, light and dark

    uint8_t game_fps = 0;

public:
    Player(uint8_t fps, float X = 0, float Y = 0, const char* P = "");
    
    void setRight(bool r);

    void setOnGround(bool og);
    bool getOnGround();

    void setOnWall(bool ow);
    bool getOnWall();

    // void setLane(uint8_t l);
    uint8_t getLane();

    bool getRhyAtk();

    bool getVertical();

    void init(SDL_Renderer *renderer);  // Used to init and reinit all the values related to 
                                        // fps or scaling (grid size) to dynamically change settings 
    
    // Player platformer movement
    void playerPlatMvt(Input *input, float dt);

    // Player rhythm movement
    void playerRhythmMvt(Input *input, float dt);

    // Player shooter movement
    // Same physics
    void playerShootMvt(Input *input, float dt);
    // Different attack style
    void playerVertAtk(Input *input, Projectile *proj, float dt);
    void playerHoriAtk(Input *input, Projectile *proj, float dt);

    ~Player();
};

#endif