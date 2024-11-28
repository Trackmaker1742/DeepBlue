#include "camera.h"

// Getters
uint16_t Camera::getRendX() { return render_x; }
uint16_t Camera::getRendY() { return render_y; }

void Camera::init(Config* config)
{
    scr_width = config->getWidth();
    scr_height = config->getHeight();

    render_x = scr_width / 2 - getGrid() / 2;
    render_y = scr_height - (getGrid() * (int(scr_height/2) / getGrid()) + getGrid());

    setX(render_x);
    setY(render_y);

    // // Set time delay until camera returns to player position
    // max_pan = false;
    // return_counter = 0;
    // return_delay = config->getFPS() * 3;

    // if (getX() < render_x) setX(render_x);
    // if (getY() < render_y) setY(render_y);
}

// Update camera
void Camera::updatePlatCam(Player *player, float dt)
{
    if (player->getEditor() == true)
        render_x = scr_width / 3 - getGrid() / 2;
    else 
        render_x = scr_width / 2 - getGrid() / 2;
    // Stop camera from going into the left, bottom edges of the screen
    if (player->getX() > render_x) 
    {
    //     if (player_prev_right != player->getRight())
    //     {
    //         dir_switch = true;
    //     }
    //     if (dir_switch)
    //     {
    //         if (!dir_counter) dir_temp = player_prev_right;
    //         setX(player->getX() + (dir_temp ? 1 : -1) * player->getGrid());
    //         dir_counter++;
    //         if (dir_counter >= dir_counter_max)
    //         {
    //             dir_counter = 0;
    //             dir_switch = false;
    //             cam_move = true;
    //         }
    //     }
    //     if (cam_move)
    //     {
    //         if (player->getRight())
    //         {
    //             if (getX() - player->getX() < player->getGrid())
    //             {
    //                 setX(getX() + (player->getVelX() / 7 + player->getGrid()) * 7 * dt);
    //             }
    //             else cam_move = false;
    //         }
    //         else
    //         {
    //             if (player->getX() - getX() < player->getGrid())
    //             {
    //                 setX(getX() + (player->getVelX() / 7 - player->getGrid()) * 7 * dt);
    //             }
    //             else cam_move = false;
    //         }
    //     }
    //     else if (!dir_switch)
    //     {
    //         setX(player->getX() + (player->getRight() ? 1 : -1) * player->getGrid());
    //     }
        setX(player->getX());
    }
    else setX(render_x);
    
    // if (abs(player->getY() - player->getGroundAnchor()) > player->getGrid()) 
    //     player->setGroundAnchor(player->getY());

    // setVelY(getGrid() * 12);

    if (player->getY() > render_y) 
    {
        if (can_move_vert) 
        {
            // if (getY() - player->getGroundAnchor() - player->getGrid() > getGrid())
            // {
            //     setY(getY() - getVelY() * dt);
            // }
            // else if (getY() - player->getGroundAnchor() - player->getGrid() < - getGrid())
            // {
            //     setY(getY() + getVelY() * dt);
            // }
            // else 
                // setY(player->getGroundAnchor() + player->getGrid() * 3);
            setY(player->getY()); 
        }
    }
    else setY(render_y);

    // Track the camera's coordinates and deal with it directly to avoid
    // jittering when entering stage corners

    player_prev_right = player->getRight();
}

// void Camera::updateRhyCam(Player *player, float dt)
// {
//     // Adjust to the x position and maintain it across all 3 lanes
//     setX(getX() + player->getRhySpeed() * dt);
//     std::cout << getX() + player->getRhySpeed() * dt << "\n";
// }

Camera::~Camera()
{ 
    std::cout << "Camera terminated!\n";
}