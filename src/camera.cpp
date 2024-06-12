#include "camera.h"

Camera::Camera(float X, float Y) : Object2d(X, Y)
{ }

// Getters
uint16_t Camera::getRendX() { return render_x; }
uint16_t Camera::getRendY() { return render_y; }

// Update camera
void Camera::updateCam(Player *player)
{
    // Stop camera from going too far into edges of the screen
    if (player->getX() > 448)
    {
        setX(player->getX());
    }
    if (player->getY() > 224)
    {
        setY(player->getY());
    }
}