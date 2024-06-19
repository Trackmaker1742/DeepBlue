#include "camera.h"

// Getters
uint16_t Camera::getRendX() { return render_x; }
uint16_t Camera::getRendY() { return render_y; }

void Camera::initCam(Scene* scene)
{
    setX(scene->getWidth()/2 - 32);
    setY(scene->getHeight()/2 - 32);

    render_x = scene->getWidth()/2 - 32;
    render_y = scene->getHeight()/2 - 32;
    
    if (getX() < 480) setX(480);
    if (getY() < 288) setY(288);
}

// Update camera
void Camera::updateCam(Player *player)
{
    // Stop camera from going too far into edges of the screen
    if (player->getX() > 480)
    {
        setX(player->getX());
    }
    if (player->getY() > 288)
    {
        setY(player->getY());
    }
}