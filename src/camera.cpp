#include "camera.h"

Camera::Camera(float X, float Y) : Object2d(X, Y)
{ }

// Getters
uint16_t Camera::getRendX() { return render_x; }
uint16_t Camera::getRendY() { return render_y; }

// Update camera
void Camera::updateCam(Player *player)
{
    setX(player->getX());
    setY(player->getY());
}