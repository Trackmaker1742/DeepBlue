#include "player.h"

Player::Player(float X, float Y, const char *P) : Object2d(X, Y, P)
{ }
    
void Player::setOnGround(bool og) { on_ground = og; }
bool Player::getOnGround() { return on_ground; }

void Player::init(SDL_Renderer *renderer)
{
    setAccelY(-1000);
    setVelX(0);
    setAccelX(400);
    initTexture(renderer);
}

void Player::playerMovement(Input *input, float dt)
{
    // Accel & decel are performed before and after setX()
    // to avoid difference in distance travelled in different frame rate

    // Speed check are performed before setVel,
    // making it impossible to break the speed limit, same goes for decel

    // Accel (First half)
    // Left
    if (input->getPress(2) && getVelX() > -vel_x_max)
    {
        right = false;
        setVelX(getVelX() - getAccelX() * dt * 0.5f);
    }
    // Right (First half accel)
    if (input->getPress(3) && getVelX() < vel_x_max)
    {
        right = true;
        setVelX(getVelX() + getAccelX() * dt * 0.5f);
    }
    // Decel (First half)
    if (getVelX() &&                                    // Speed != 0
        input->getPress(2) && input->getPress(3) ||     // Dual input
        !input->getPress(2) && !input->getPress(3) ||   // No input
        input->getPress(2) && getVelX() > 0 ||          // Go left when speed > 0
        input->getPress(3) && getVelX() < 0)            // Go right when speed < 0
    {
        // Decel works by applying 2.5 times the speed of the opposite direction
        // Left
        if (getVelX() < 0)
        {
            temp_speed_right = getVelX() + getAccelX() * dt * 0.5f * 2.5;
            setVelX(temp_speed_right < 0 ? temp_speed_right : 0);
        }
        // Right
        else if (getVelX() > 0)
        {
            temp_speed_left = getVelX() - getAccelX() * dt * 0.5f * 2.5;
            setVelX(temp_speed_left > 0 ? temp_speed_left : 0);
        }
    }

    // Horizontal movement calculation
    setX(getX() + getVelX() * dt);  
    
    // Accel (Second half)
    // Left
    if (input->getPress(2) && getVelX() > -vel_x_max)
    {
        right = false;
        setVelX(getVelX() - getAccelX() * dt * 0.5f);
    }
    // Right
    if (input->getPress(3) && getVelX() < vel_x_max)
    {
        right = true;
        setVelX(getVelX() + getAccelX() * dt * 0.5f);
    }
    // Decel (Second half)
    if (getVelX() &&                                    // Speed != 0
        input->getPress(2) && input->getPress(3) ||     // Dual input
        !input->getPress(2) && !input->getPress(3) ||   // No input
        input->getPress(2) && getVelX() > 0 ||          // Go left when speed > 0
        input->getPress(3) && getVelX() < 0)            // Go right when speed < 0
    {
        // Decel works by applying 2.5 times the speed of the opposite direction
        // Left
        if (getVelX() < 0)
        {
            temp_speed_right = getVelX() + getAccelX() * dt * 0.5f * 2.5;
            setVelX(temp_speed_right < 0 ? temp_speed_right : 0);
        }
        // Right
        else if (getVelX() > 0)
        {
            temp_speed_left = getVelX() - getAccelX() * dt * 0.5f * 2.5;
            setVelX(temp_speed_left > 0 ? temp_speed_left : 0);
        }
    }

    // Speed cap
    if (getVelX() < -vel_x_max) setVelX(-vel_x_max);
    if (getVelX() > vel_x_max) setVelX(vel_x_max);

    // std::cout << getX() << " " << getY() << "\n";

    // Jump
    if (input->getPress(4) && on_ground)
    {
        input->setHold(4, false);
        setVelY(700);
        on_ground = false;
    }

    if (getVelY() < vel_terminal)
    {
        setVelY(vel_terminal);
    }

    // Movement test
    // Aerial velocity control
    if (!on_ground)
    {
        setVelY(getVelY() + getAccelY() * dt * 0.5f);
        setY(getY() + getVelY() * dt);
        setVelY(getVelY() + getAccelY() * dt * 0.5f);
    }
    else
    {
        setVelY(0);
    }

    // // Dev mode
    // if (input->getPress(11))
    // {
    //     setY(getY() + 10);
    // }
    // if (input->getPress(12))
    // {
    //     setY(getY() - 3);
    // }
    // if (input->getPress(13))
    // {
    //     setX(getX() - 3);
    // }
    // if (input->getPress(14))
    // {
    //     setX(getX() + 3);
    // }
}