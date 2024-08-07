#include "player.h"

Player::Player(uint8_t fps, float X, float Y, const char *P) : game_fps(fps), Object2d(X, Y, P)
{ }
    
void Player::setRight(bool r) { right = r; }    

void Player::setOnGround(bool og) { on_ground = og; }
bool Player::getOnGround() { return on_ground; }

void Player::setOnWall(bool ow) { on_wall = ow; }
bool Player::getOnWall() { return on_wall; }

bool Player::getInvul() { return invul; }

uint8_t Player::getLane() { return current_lane; }
void Player::setRhyBar(float bar) { rhythm_bar = bar; } 
float Player::getRhyBar() { return rhythm_bar; }
bool Player::getRhyAtk() { return rhythm_atk; }
float Player::getRhySpeed() { return rhythm_speed; }
float Player::getRhyAtkX() { return rhythm_atk_x; }
float Player::getRhyAtkY() { return rhythm_atk_y; }
uint16_t Player::getRhyAtkGrid() { return rhythm_atk_grid; }

bool Player::getVertical() { return vertical; }

void Player::initPlat(SDL_Renderer *renderer)
{
    setGrid(64);

    on_ground = false;
    right = true;

    // Velocity, acceleration
    vel_terminal = -getGrid() * 17;
    vel_x_max = getGrid() * 8;
    setAccelX(getGrid() * 5);
    setAccelY(-getGrid() * 30);

    // Dash
    can_dash = true;
    on_dash = false;
    on_dash_delay = false;
    dash_counter = 0;
    dash_frame_max = game_fps / 5;
    dash_frame_delay = game_fps / 2;

    // Wall
    on_wall = false;
    on_wall_jump = false;
    wall_jump_counter = 0;
    wall_climb_speed = getGrid() * 1.5;
    wall_jump_frame_max = game_fps / 5;

    // Jump / double jump
    jump_count = 2;

    initTexture(renderer);
}

void Player::initRhythm(SDL_Renderer *renderer)
{
    setGrid(128);

    on_ground = false;

    // Invul period after taking damage
    invul = false;
    invul_counter = 0;
    invul_frame_max = game_fps;

    current_lane = 2;

    // Dash
    dash_frame_max = game_fps / 5;
    dash_frame_delay = game_fps / 2;

    // Attack
    rhythm_bar = 0;
    rhythm_bar_regen_rate = 8;
    rhythm_speed = getGrid() * 8;
    rhythm_can_atk = false;
    rhythm_atk = false;
    rhythm_atk_counter = 0;
    rhythm_atk_frame_max = game_fps / 5;
    rhythm_atk_grid = 96;   // 1.5 times the size of a normal grid

    initTexture(renderer);
}

void Player::initVertShooter(SDL_Renderer *renderer)
{
    setGrid(64);

    // Invul period after taking damage
    invul = false;
    invul_counter = 0;
    invul_frame_max = game_fps;

    initTexture(renderer);
}

void Player::initHoriShooter(SDL_Renderer *renderer)
{
    setGrid(128);

    // Invul period after taking damage
    invul = false;
    invul_counter = 0;
    invul_frame_max = game_fps;

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
    if (on_wall_jump) wall_jump_counter++;
    if (wall_jump_counter >= wall_jump_frame_max) 
    {
        wall_jump_counter = 0;
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
        dash_counter++;

        if (dash_counter < dash_frame_max) setVelX((right ? 1 : -1) * getGrid() * 20);
        else if (dash_counter == dash_frame_max)
        {
            on_dash = false;
            on_dash_delay = true;
            setVelX((right ? 1 : -1) * vel_x_max);
        }
    } 
    if (on_dash_delay)
    {
        dash_counter++;
        
        if (dash_counter >= dash_frame_delay)
        {
            on_dash_delay = false;
            dash_counter = 0;
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
    right = true;
    // Constant default camera speed, player can speed up or slow down
    // as they want, invisible wall on left and right boundary
    setVelX(rhythm_speed);
    // Set hurtbox location
    // About the attack hurtbox
    // Since the player grid will be around 128x128
    // The hurtbox will have to be in front of that
    // Each # or * is 32x32
    // # represent the player hitbox
    // * represent the hurtbox
    // This can be adjusted later on
    // Visual demonstration
    // ####***
    // ####***
    // ####***
    // ####
    rhythm_atk_x = getX() + getGrid();
    rhythm_atk_y = getY() + getGrid() * 0.25;

    // Refilling bar (kinda like in osu, I think?)
    if (rhythm_bar < 100) { rhythm_bar += rhythm_bar_regen_rate * dt; }

    // Concept:
    // 3 obstacle type: water geyser (low and high), break-able object
    // Quick step inspired, dodge by going up (left) and down (right)
    // Attack to break apart object
    // Jump to bypass low geyser
    // Dodge to avoid high geyser
    
    // Lane switching
    if (input->getPress(0) && current_lane < 3)
    {
        input->setHold(0, false);
        setX(getX() + getGrid());
        setY(getY() + getGrid() * 3);
        current_lane++;
    }
    if (input->getPress(1) && current_lane > 1)
    {
        input->setHold(1, false);
        setX(getX() - getGrid());
        setY(getY() - getGrid() * 3);
        current_lane--;
    }

    // Horizontal movement
    // Left and right movement
    if (!on_dash && input->getPress(2))
    {
        setVelX(getVelX() - getGrid() * 4);
    }
    if (!on_dash && input->getPress(3))
    {
        setVelX(getVelX() + getGrid() * 4);
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
        dash_counter++;

        if (dash_counter < dash_frame_max) setVelX(getVelX() + getGrid() * 10);
        else if (dash_counter == dash_frame_max)
        {
            on_dash = false;
            on_dash_delay = true;
            // Remember to set this back to 
            // default rhythm speed after dash
            setVelX((right ? 1 : -1) * 0);
        }
    } 
    if (on_dash_delay)
    {
        dash_counter++;
        
        if (dash_counter >= dash_frame_delay)
        {
            on_dash_delay = false;
            dash_counter = 0;
        }
    }
    // Horizontal movement calculation
    setX(getX() + getVelX() * dt);  

    // Vertical movement + gravity
    // Jump
    if (input->getPress(4))
    {
        input->setHold(4, false);
        setVelY(getGrid() * 12);
        on_ground = false;
    }
    for (int i = 1; i < 4; i++)
    {
        if (current_lane == i && getY() < getGrid() * i * 2)
        {
            setY(getGrid() * i * 2);
            on_ground = true;
        }
    }
    // Gravity
    if (on_ground || on_dash)
    {
        setVelY(0);
        can_dash = true;
    }
    else 
    {
        setVelY(getVelY() + getAccelY() * dt * 0.5f);
        setY(getY() + getVelY() * dt);
        setVelY(getVelY() + getAccelY() * dt * 0.5f);
    }

    // Attack
    if (input->getPress(6) && rhythm_can_atk)
    {
        input->setHold(6, false);
        rhythm_atk = true;
        rhythm_can_atk = false;
    }
    if (rhythm_atk)
    {
        rhythm_atk_counter++;
        if (rhythm_atk_counter >= rhythm_atk_frame_max)
        {
            rhythm_atk = false;
            rhythm_atk_delay = true;
        }   
    }
    if (rhythm_atk_delay)
    {
        rhythm_atk_counter++;
        if (rhythm_atk_counter >= rhythm_atk_delay)
        {
            rhythm_atk_counter = 0;
            rhythm_atk_delay = false;
            rhythm_can_atk = true;
        }
    }

    // Invul
    if (invul)
    {
        invul_counter++;
        if (invul_counter >= invul_frame_max) invul = false;
    }
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
void Player::playerVertAtk(Input *input, Projectile *proj, float dt)
{
    // Allows for hold, no need to spam attack to shoot
    // An interval of half a second for default projectile speed
    // and a quarter of a second for stage 1 and beyond
    if (input->getPress(6))
    {
        // Create a projectile, huh, new object I guess
    }
}
void Player::playerHoriAtk(Input *input, Projectile *proj, float dt)
{

}

Player::~Player()
{ }