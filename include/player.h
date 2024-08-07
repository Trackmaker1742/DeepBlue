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
    uint8_t dash_counter = 0;
    float dash_frame_max;     // = fps / 5
    float dash_frame_delay;   // = fps / 2

    // Wall related variables
    bool on_wall = false;
    float wall_climb_speed;
    bool on_wall_jump = false;
    uint8_t wall_jump_counter = 0;
    float wall_jump_frame_max;

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
    // A meter will regenerate over time, maxing out at 100
    // Taking damage will lower the meter by 30
    // Meaning 4 consecutive hit will end the game
    float rhythm_bar;
    float rhythm_bar_regen_rate;
    uint8_t current_lane;   // 1 is the lowest, 3 is the highest
                                //     3
                                //     3
                                //   2
                                //   2                                
                                // 1
                                // 1
                                // Kinda like this
                                // 2 rows because the sprite will be 128x128
    float rhythm_speed;     // Default speed during rhythm mode
    // Rhythm attack creates a hurtbox in front of the player
    // This block will have its own block collision
    bool rhythm_can_atk = false;
    bool rhythm_atk = false;
    uint8_t rhythm_atk_counter = 0;
    float rhythm_atk_frame_max;     // = fps / 5
    float rhythm_atk_delay;         // = fps / 2

    float rhythm_atk_x;
    float rhythm_atk_y;
    uint16_t rhythm_grid;       // Hurtbox size, square because I'm lazy

    // Shooter stuff
    // Will need a different hitbox for both as well
    bool vertical = true;   // To identify each mode during collision calc
    // Vertical
    uint8_t level = 0;      // Increase as more item is collected, max is 4
    // Horizontal
    uint8_t health = 3;     // 3 hp, restart at 0, can be refilled with item
    uint16_t energy = 0;    // Increase as more shot lands, max 300?
    bool dark = true;       // 2 states, light and dark

    // For cooldown handler and the likes
    // I could use dt but that isn't as precise
    // While a fraction of a second is a lot easier to visualize
    uint8_t game_fps = 0;

public:
    Player(uint8_t fps, float X = 0, float Y = 0, const char* P = "");
    
    void setRight(bool r);

    void setOnGround(bool og);
    bool getOnGround();

    void setOnWall(bool ow);
    bool getOnWall();

    uint8_t getLane();

    bool getRhyAtk();
    float getRhySpeed();

    bool getVertical();

    void initPlat(SDL_Renderer *renderer);      // Used to init and reinit all the values related to 
                                                // fps or scaling (grid size) to dynamically change settings 
    
    void initRhythm(SDL_Renderer *renderer);
    void initVertShooter(SDL_Renderer *renderer);
    void initHoriShooter(SDL_Renderer *renderer);

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