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
public:
    // Getters
    uint16_t getRendX();
    uint16_t getRendY();

    void init(Scene* scene);

    // Update platformer camera
    void updatePlatCam(Player *player);

    // Update rhythm camera
    void updateRhyCam(Player *player);
};

#endif