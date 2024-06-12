#ifndef CAMERA_H
#define CAMERA_H

#include "scene.h"
#include "object2d.h"
#include "player.h"

class Camera : public Object2d
{
private:
    uint16_t render_x = Scene::WIDTH/2 - 32;
    uint16_t render_y = Scene::HEIGHT/2 - 32;
public:
    Camera(float X = Scene::WIDTH/2, float Y = Scene::HEIGHT/2);

    // Getters
    uint16_t getRendX();
    uint16_t getRendY();

    void initCam();

    // Update camera
    void updateCam(Player *player);
};

#endif