#include "player.h"

Player::Player(uint8_t fps, float X, float Y, const char *P) : game_fps(fps), Object2d(X, Y, P)
{ }
    
void Player::setRight(bool r) { right = r; }    

void Player::setOnGround(bool og) { on_ground = og; }
bool Player::getOnGround() { return on_ground; }

void Player::setOnWall(bool ow) { on_wall = ow; }
bool Player::getOnWall() { return on_wall; }

// void Player::setLane(uint8_t l) { current_lane = l; }
uint8_t Player::getLane() { return current_lane; }

bool Player::getRhyAtk() { return rhythm_atk; }

void Player::init(SDL_Renderer *renderer)
{
    setAccelY(-getGrid() * 30);
    vel_terminal = -getGrid() * 17;
    vel_x_max = getGrid() * 8;
    setAccelX(getGrid() * 5);

    dash_frame_max = game_fps / 5;
    dash_frame_delay = game_fps / 2;

    wall_climb_speed = getGrid();
    wall_jump_frame_max = game_fps / 5;

    initTexture(renderer);
}

// Player platformer movement
void Player::playerPlatMvt(Input *input, float dt)
{
    // Accel & decel are performed before and after setX()
    // to avoid difference in distance travelled in different frame rate

    // Speed check are performed before setVel,
    // making it impossible to break the speed limit, same goes for decel

    // Accel (First half)
    // Left
    if (!on_wall && !on_dash && input->getPress(2) && getVelX() > -vel_x_max)
    {
        right = false;
        setVelX(getVelX() - getAccelX() * dt * 0.5f);
    }
    // Right (First half accel)
    if (!on_wall && !on_dash && input->getPress(3) && getVelX() < vel_x_max)
    {
        right = true;
        setVelX(getVelX() + getAccelX() * dt * 0.5f);
    }
    // Decel (First half)
    if (!on_wall && !on_dash &&             // Not on wall, not currently dashing
        getVelX() &&                                    // Speed != 0
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
    if (!on_wall && !on_dash && input->getPress(2) && getVelX() > -vel_x_max)
    {
        right = false;
        setVelX(getVelX() - getAccelX() * dt * 0.5f);
    }
    // Right
    if (!on_wall && !on_dash && input->getPress(3) && getVelX() < vel_x_max)
    {
        right = true;
        setVelX(getVelX() + getAccelX() * dt * 0.5f);
    }
    // Decel (Second half)
    if (!on_wall && !on_dash &&                                     // Not on wall
        getVelX() &&                                    // Speed != 0
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
    if (getVelX() < -vel_x_max && !on_wall_jump) setVelX(-vel_x_max);
    if (getVelX() > vel_x_max && !on_wall_jump) setVelX(vel_x_max);
    
    // Reset stuff while on the ground
    if (on_ground) 
    {
        can_dash = true;
        jump_count = 2;
    }

    // Wall climb input handler
    if (on_wall)
    {
        if (input->getPress(0))
        {
            // Vertical movement calculation
            setY(getY() + wall_climb_speed * dt);  
        }
        if (input->getPress(1))
        {
            // Vertical movement calculation
            setY(getY() - wall_climb_speed * dt);  
        }
        can_dash = true;
        jump_count = 1;
    }

    // Jump / double jump / wall jump
    if (input->getPress(4) && jump_count > 0)
    {
        if (on_ground)
        {
            input->setHold(4, false);
            setVelY(getGrid() * 15);
            jump_count--;
            on_ground = false;
        }
        else if (!on_wall)
        {
            input->setHold(4, false);
            setVelY(getGrid() * 12);
            jump_count--;
        }
        if (on_wall)
        {
            input->setHold(4, false);
            setX(getX() + (right ? -1 : 1) * 10);
            setVelX((right ? -1 : 1) * getGrid() * 10);
            setVelY(getGrid() * 12);
            jump_count--;
            on_wall = false;
            on_wall_jump = true;
        }
    }
    if(on_wall_jump) wall_jump_frame++;
    if(wall_jump_frame >= wall_jump_frame_max) 
    {
        wall_jump_frame = 0;
        on_wall_jump = false;
    }
    if (getVelY() < vel_terminal)
    {
        setVelY(vel_terminal);
    }

    // Movement test
    // Aerial velocity control
    if (on_ground || on_dash || on_wall)
    {
        setVelY(0); 
    }
    else 
    {
        if (jump_count == 2) jump_count--;
        setVelY(getVelY() + getAccelY() * dt * 0.5f);
        setY(getY() + getVelY() * dt);
        setVelY(getVelY() + getAccelY() * dt * 0.5f);
    }

    // Dash
    if (input->getPress(5) && can_dash && !on_dash)
    {
        input->setHold(5, false);
        can_dash = false;
        on_dash = true;
    }
    if (on_dash)
    {
        dash_frame++;

        if (dash_frame < dash_frame_max) setVelX((right ? 1 : -1) * getGrid() * 20);
        else if (dash_frame == dash_frame_max) 
        {
            on_dash = false;
            on_dash_delay = true;
            setVelX((right ? 1 : -1) * vel_x_max);
        }
    } 
    if (on_dash_delay)
    {
        dash_frame++;
        
        if (dash_frame >= dash_frame_delay)
        {
            on_dash_delay = false;
            dash_frame = 0;
        }
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

// Player rhythm movement
void Player::playerRhythmMvt(Input *input, float dt)
{
    // Constant speed, no stopping
    // setVelX(getVelX() + getAccelX() * dt * 0.5f);
    // setX(getX() + getVelX() * dt);  
    // setVelX(getVelX() + getAccelX() * dt * 0.5f);

    // Concept:
    // 3 obstacle type: water geyser (low and high), break-able object
    // Quick step inspired, dodge by going up (left) and down (right)
    // Attack to break apart object
    // Jump to bypass low geyser
    // Dodge to avoid high geyser

    if (input->getPress(0) && current_lane < 3)
    {
        input->setHold(0, false);
        setX(getX() + getGrid());
        setY(getY() + getGrid());
        current_lane++;
    }
    if (input->getPress(1) && current_lane > 1)
    {
        input->setHold(1, false);
        setX(getX() - getGrid());
        setY(getY() - getGrid());
        current_lane--;
    }
    // Collision first, then jump and attack later
    if (input->getPress(4))
    {
        input->setHold(4, false);
        setVelY(getGrid() * 10);
        on_ground = false;
    }
    for (int i = 1; i < 4; i++)
    {
        if (current_lane == i && getY() < getGrid() * i)
        {
            setY(getGrid() * i);
            on_ground = true;
        }
    }
    // Gravity
    if (!on_ground)
    {
        setVelY(getVelY() + getAccelY() * dt * 0.5f);
        setY(getY() + getVelY() * dt);
        setVelY(getVelY() + getAccelY() * dt * 0.5f);
    }
    else setVelY(0);
    std::cout << getY() << " " << getGrid() << "\n";
}

// Player shooter movement
// Same physics
void Player::playerShootMvt(Input *input, float dt)
{
    // Gotta handle dual input, huh
    // Diagonal movement needs to be controlled

    // 4 directional movement
    if (input->getPress(0) && 
    !input->getPress(1) && !input->getPress(2) && !input->getPress(3))
    {
        setY(getY() + getGrid() * dt);
    }
    if (input->getPress(1) &&
    !input->getPress(0) && !input->getPress(2) && !input->getPress(3))
    {
        setY(getY() - getGrid() * dt);
    }
    if (input->getPress(2) &&
    !input->getPress(1) && !input->getPress(0) && !input->getPress(3))
    {
        setX(getX() - getGrid() * dt);
    }
    if (input->getPress(3) &&
    !input->getPress(1) && !input->getPress(2) && !input->getPress(0))
    {
        setX(getX() + getGrid() * dt);
    }

    // The other 4 diagonal movement
    // Up left
    if (input->getPress(0) && 
    !input->getPress(1) && input->getPress(2) && !input->getPress(3))
    {
        setY(getY() + (getGrid() / 2) * dt);
        setX(getX() - (getGrid() / 2) * dt);
    }
    // Up right
    if (input->getPress(0) && 
    !input->getPress(1) && !input->getPress(2) && input->getPress(3))
    {
        setY(getY() + (getGrid() / 2) * dt);
        setX(getX() + (getGrid() / 2) * dt);
    }
    // Down left
    if (!input->getPress(0) && 
    input->getPress(1) && input->getPress(2) && !input->getPress(3))
    {
        setY(getY() - (getGrid() / 2) * dt);
        setX(getX() - (getGrid() / 2) * dt);
    }
    // Down right
    if (!input->getPress(0) && 
    input->getPress(1) && !input->getPress(2) && input->getPress(3))
    {
        setY(getY() - (getGrid() / 2) * dt);
        setX(getX() + (getGrid() / 2) * dt);
    }
}
// Different attack style
void Player::playerVertAtk(Input *input, float dt)
{
    // Allows for hold, no need to spam attack to shoot
    if (input->getPress(6))
    {
        // Create a projectile, huh, new object I guess
    }
}
void Player::playerHoriAtk(Input *input, float dt)
{

}

Player::~Player()
{ }