#include "camera.h"

// Getters
uint16_t Camera::getRendX() { return render_x; }
uint16_t Camera::getRendY() { return render_y; }

void Camera::init(Scene* scene)
{
    render_x = scene->getWidth()/2 - getGrid() / 4;
    render_y = scene->getHeight()/2 - getGrid();

    setX(render_x);
    setY(render_y);

    // // Set time delay until camera returns to player position
    // max_pan = false;
    // return_counter = 0;
    // return_delay = scene->getFPS() * 3;

    // if (getX() < render_x) setX(render_x);
    // if (getY() < render_y) setY(render_y);
}

// Update camera
void Camera::updatePlatCam(Player *player, float dt)
{
    // Stop camera from going into the left, bottom edges of the screen
    if (player->getX() > render_x) { setX(player->getX()); }
    else setX(render_x);
    if (player->getY() > render_y) { setY(player->getY()); }
    else setY(render_y);
}

void Camera::updateRhyCam(Player *player, float dt)
{
    // Adjust to the x position and maintain it across all 3 lanes
    setX(getX() + player->getRhySpeed() * dt);
    std::cout << getX() + player->getRhySpeed() * dt << "\n";
}

Camera::~Camera()
{ }