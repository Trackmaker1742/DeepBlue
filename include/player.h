#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <math.h>
#include <memory>

#include "object2d.h"
#include "input.h"
#include "projectile.h"
#include "audio.h"

// Time to split up player and have it interact with renderer better
// Player class can handle all 3 game mode, with different attribute for each
// Grid is used as an unit of measurement, 
// allowing for dynamic game update and more consistent calculation

class Player : public Object2d
{
private:
    // Collision handler, will be replacing grid
    uint16_t width;
    uint16_t height;

    float center_x;
    float center_y;

    // Debug mode
    bool editor;

    bool on_ground;
    bool right;     // true: facing right
                    // false: facing left

    // Max velocity, both x and y
    float vel_terminal;
    float vel_x_max;
    float vel_y_max;    // Shooter mode only 

    // No jitter decel
    float temp_speed_left;
    float temp_speed_right;
    // For shooter mode only
    float temp_speed_up;
    float temp_speed_down;

    // Dash related variables
    bool can_dash;
    bool on_dash;
    bool on_dash_delay;
    uint8_t dash_counter;
    float dash_frame_max;     // = fps / 5
    float dash_frame_delay;   // = fps / 2

    // Wall related variables
    bool on_wall;
    float wall_climb_speed;
    bool on_wall_jump;
    uint8_t wall_jump_counter;
    float wall_jump_frame_max;

    // Jump / double jump related variables
    int jump_count;

    // On block variable, to deal with standing on block collision
    bool on_moving_block;

    // Spring related variables
    // Only used for horizontal springs (left and right)
    bool on_spring;
    uint8_t spring_counter;
    float spring_frame_max;     // = fps / 7

    // Camera variables
    float ground_anchor;

    // Invul state, used for rhythm and shooter
    // 1 second is the max invul time
    bool invul;
    uint16_t invul_counter;
    float invul_frame_max;  // = fps

    // // Rhythm stuff
    // // A meter will regenerate over time, maxing out at 100
    // // Taking damage will lower the meter by 30
    // // Meaning 4 consecutive hit will end the game
    // float rhythm_bar;
    // float rhythm_bar_regen_rate;
    // uint8_t current_lane;   // 1 is the lowest, 3 is the highest
    //                             //     3
    //                             //     3
    //                             //   2
    //                             //   2                                
    //                             // 1
    //                             // 1
    //                             // Kinda like this
    //                             // 2 rows because the sprite will be 128x128
    // float rhythm_speed;     // Default speed during rhythm mode
    // // Rhythm attack creates a hurtbox in front of the player
    // // This block will have its own block collision
    // bool rhythm_can_atk;
    // bool rhythm_atk;
    // uint8_t rhythm_atk_counter;
    // float rhythm_atk_frame_max;     // = fps / 5
    // float rhythm_atk_delay;         // = fps / 2

    // float rhythm_atk_x;
    // float rhythm_atk_y;
    // uint16_t rhythm_atk_grid;       // Hurtbox size, square because I'm lazy

    // Shooter stuff
    // Will need a different hitbox for both as well
    bool vertical = true;   // To identify each mode during collision calc
    bool shooter_can_atk;
    uint8_t shooter_atk_counter;
    float shooter_atk_delay;
    std::vector<Projectile*> projectiles;
    // Vertical
    uint8_t level;      // Increase as more item is collected, max is 4
    // Horizontal
    float smallbox_x[3];  // Array of x coordinates (for 3 different hitboxes)
    float smallbox_y[3];  // Array of y coordinates (for 3 different hitboxes)
    uint8_t smallbox_width;     // Width of each small hitbox
    uint8_t smallbox_height;    // Height of each small hitbox
    uint8_t health;     // 3 hp, restart at 0, can be refilled with item
    uint16_t energy;    // Increase as more shot lands, max 300?
    bool light;         // 2 states, light and dark
    bool hold_light;
    bool hold_dark;

    // For cooldown handler and the likes
    // I could use dt but that isn't as precise
    // While a fraction of a second is a lot easier to visualize
    float game_fps;

    // Hitbox and sprite stuff
    int sprite_state;
    float render_x;
    float render_y;

    // Variables for ground jump/ascend/descend rendering
    // Jump start, apex, land work based on a sudden shift in velocity
    // Ascend, descend work based differences between current and previous y coordinates
    bool jump_start;
    bool jump;
    float jump_counter;
    bool ascend;
    bool descend;
    bool apex;      // Apex of a jump or ascend, to render the transition
    bool land;
    bool climb_up;
    bool climb_down;
    bool dash_halt;

    // Sprite paths
    const char *platformer_path = "res/Character Sheets/nakuru_sheet.png";
    const char *vert_shooter_path = "res/Character Sheets/itsuki_sheet.png";
    const char *hori_shooter_path = "res/Character Sheets/la_priere_sheet.png";
    const char *rhythm_path = "res/Character Sheets/sky_delta_sheet.png";

public:
    Player(float X = 0, float Y = 0, const char *P = "");
    
    uint16_t getWidth();
    uint16_t getHeight();

    float getCenterX();
    float getCenterY();

    void setEditor(bool e);
    bool getEditor();

    float getVelXMax();

    void setRight(bool r);
    void setOnGround(bool og);
    void setOnWall(bool ow);
    void setOnMovingBlock(bool omb);
    void setOnSpring(bool os);
    void setDashHalt(bool dh);
    void setGroundAnchor(float ga);

    bool getRight();
    bool getOnGround();
    bool getOnWall();
    bool getOnMovingBlock();
    bool getDashHalt();
    bool getOnDash();
    float getGroundAnchor();

    // Anything with health and invul state
    void setInvul(bool i);
    bool getInvul();

    // // Rhythm related variables
    // void setRhyBar(float bar);
    // float getRhyBar();
    // uint8_t getLane();
    // bool getRhyAtk();
    // float getRhySpeed();
    // float getRhyAtkX();
    // float getRhyAtkY();
    // uint16_t getRhyAtkGrid();

    // Shooter related variables
    bool getVertical();

    void setRenderX(float x);
    void setRenderY(float y);
    float getRenderX();
    float getRenderY();

    void setJumpStart(bool js);
    void setAscend(bool a);
    void setDescend(bool d);
    void setApex(bool a);
    void setLand(bool l);

    bool getJumpStart();
    bool getAscend();
    bool getDescend();
    bool getApex();
    bool getLand();
    bool getClimbUp();
    bool getClimbDown();

    void setHealth(uint8_t h);
    uint8_t getHealth();
    // void setSmallboxX(int i, float x);
    // void setSmallboxY(int i, float y);
    float getSmallboxX(int i);
    float getSmallboxY(int i);
    float getSmallboxWidth();
    float getSmallboxHeight();

    std::vector<Projectile*> getProjectiles();
    
    void initPlat(Config *config);      // Used to init and reinit all the values related to 
                                                // fps or scaling (grid size) to dynamically change settings 
    // void initRhythm(Config *config);
    void initVertShooter(Config *config);
    void initHoriShooter(Config *config);

    // Player platformer movement
    void resetMoves();
    void stopDash();
    void springGravity();
    void platformerMvtAccel(Input *input, float dt);
    void platformerMvt(Input *input, float dt);

    // Player shooter movement
    void shooterMvtAccel(Input *input, float dt);
    // Same physics
    void shooterMvt(Input *input, float dt, uint16_t game_width, uint16_t game_height);
    // Different attack style
    void shooterVertAtk(Config *config, Input *input, float dt);
    void shooterHoriAtk(Config *config, Input *input, float dt);

    // // Player rhythm movement
    // void rhythmMvt(Input *input, float dt);

    // For stage edit mode
    void editorMvt(Input *input, float dt);

    void unload();

    ~Player();
};

#endif