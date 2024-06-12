#include "camera.h"

Camera::Camera(float X, float Y) : Object2d(X, Y)
{ }

// Getters
uint16_t Camera::getRendX() { return render_x; }
uint16_t Camera::getRendY() { return render_y; }

void Camera::initCam()
{
    if (getX() > 480) setX(480);
    if (getY() > 288) setY(288);
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