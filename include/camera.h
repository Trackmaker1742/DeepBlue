#ifndef CAMERA_H
#define CAMERA_H

#include "scene.h"
#include "object2d.h"
#include "player.h"

class Camera : public Object2d
{
private:
    uint16_t render_x;
    uint16_t render_y;

    // float prev_vel_x;

    // bool max_pan;   // Check to see if camera is at maximum distance from player
    // uint8_t return_counter;
    // uint8_t return_delay;

    bool can_move_vert = true;
    bool can_move_hori = true;

    bool player_prev_right;
    bool dir_temp;
    float dir_counter;
    uint16_t dir_counter_max = 75 * 2;
    bool dir_switch = true;
    bool cam_move = false;

public:
    // Getters
    uint16_t getRendX();
    uint16_t getRendY();

    void init(Scene* scene);

    // Update platformer camera
    void updatePlatCam(Player *player, float dt);

    // Update rhythm camera
    void updateRhyCam(Player *player, float dt);

    ~Camera();
};

#endif