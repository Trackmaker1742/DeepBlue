#include "player.h"

Player::Player(uint8_t fps, float X, float Y, const char *P) : 
    Object2d(X, Y, P), game_fps(fps)
{ }
    
uint16_t Player::getWidth() { return width; }
uint16_t Player::getHeight() { return height; }

float Player::getVelXMax() { return vel_x_max; }

void Player::setRight(bool r) { right = r; }
void Player::setOnGround(bool og) { on_ground = og; }
void Player::setOnWall(bool ow) { on_wall = ow; }
void Player::setOnMovingBlock(bool omb) { on_moving_block = omb; }
void Player::setDashHalt(bool dh) { dash_halt = dh; }

bool Player::getRight() { return right; }
bool Player::getOnGround() { return on_ground; }
bool Player::getOnWall() { return on_wall; }
bool Player::getOnMovingBlock() { return on_moving_block; }
bool Player::getDashHalt() { return dash_halt; }
bool Player::getOnDash() { return on_dash; }

void Player::setInvul(bool i) { invul = i; }
bool Player::getInvul() { return invul; }

void Player::setRhyBar(float bar) { rhythm_bar = bar; } 
float Player::getRhyBar() { return rhythm_bar; }
uint8_t Player::getLane() { return current_lane; }
bool Player::getRhyAtk() { return rhythm_atk; }
float Player::getRhySpeed() { return rhythm_speed; }
float Player::getRhyAtkX() { return rhythm_atk_x; }
float Player::getRhyAtkY() { return rhythm_atk_y; }
uint16_t Player::getRhyAtkGrid() { return rhythm_atk_grid; }

bool Player::getVertical() { return vertical; }

void Player::setRenderX(float x) { render_x = x; }
void Player::setRenderY(float y) { render_y = y; }
float Player::getRenderX() { return render_x; }
float Player::getRenderY() { return render_y; }

void Player::setJumpStart(bool js) { jump_start = js; }
void Player::setAscend(bool a) { ascend = a; }
void Player::setDescend(bool d) { descend = d; }
void Player::setApex(bool a) { apex = a; }
void Player::setLand(bool l) { land = l; }

bool Player::getJumpStart() { return jump_start; }
bool Player::getAscend() { return ascend; }
bool Player::getDescend() { return descend; }
bool Player::getApex() { return apex; }
bool Player::getLand() { return land; }
bool Player::getClimbUp() { return climb_up; }
bool Player::getClimbDown() { return climb_down; }

void Player::setHealth(uint8_t h) { health = h; }
uint8_t Player::getHealth() { return health; }
// void Player::setSmallboxX(int i, float x) { smallbox_x[i] = x; }
// void Player::setSmallboxY(int i, float y) { smallbox_y[i] = y; }
float Player::getSmallboxX(int i) { return smallbox_x[i]; }
float Player::getSmallboxY(int i) { return smallbox_y[i]; }
float Player::getSmallboxWidth() { return smallbox_width; }
float Player::getSmallboxHeight() { return smallbox_height; }

std::vector<Projectile*> Player::getProjectiles() { return projectiles; }

void Player::initPlat(SDL_Renderer *renderer)
{
    setGrid(128);

    // Hitbox
    width = getGrid() * 0.40625;
    height = getGrid() * 0.625;

    on_ground = false;
    // Rendering stuff
    right = true;

    // Velocity, acceleration
    vel_terminal = -getGrid() * 17;
    vel_x_max = getGrid() * 6;
    setAccelX(getGrid() * 4);
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
    wall_climb_speed = getGrid() * 1.2;
    wall_jump_frame_max = game_fps / 5;

    // Jump / double jump
    jump_count = 2;

    on_moving_block = false;

    jump_start = false;
    jump = false;
    jump_counter = 0;
    ascend = false;
    apex = false;
    descend = false;
    land = false;
    climb_up = false;
    climb_down = false;
    dash_halt = false;

    initTexture(platformer_path, renderer);
}

void Player::initVertShooter(SDL_Renderer *renderer)
{
    setGrid(96);
    width = getGrid();
    height = getGrid();

    // Max velocity
    setAccelX(getGrid() * 4);
    vel_x_max = getGrid() * 4;
    vel_y_max = vel_x_max;
 
    // Invul period after taking damage
    invul = false;
    invul_counter = 0;
    invul_frame_max = game_fps;

    // Attack
    shooter_can_atk = true;
    shooter_atk_counter = 0;
    shooter_atk_delay = game_fps / 5;

    // Player status
    level = 0;

    initTexture(vert_shooter_path, renderer);
}

void Player::initHoriShooter(SDL_Renderer *renderer)
{
    setGrid(96 * 2);
    width = getGrid() * 2/3;
    height = getGrid() * 2/3;
    smallbox_width = getGrid() * 13/48;
    smallbox_height = getGrid() * 19/48;
    
    // Max velocity
    setAccelX(getGrid() * 4);
    vel_x_max = getGrid() * 4;
    vel_y_max = vel_x_max;

    // Invul period after taking damage
    invul = false;
    invul_counter = 0;
    invul_frame_max = game_fps;

    // Attack
    shooter_can_atk = true;
    shooter_atk_counter = 0;
    shooter_atk_delay = game_fps / 15;

    // Player status
    health = 3;
    energy = 0;
    dark = true;

    initTexture(hori_shooter_path, renderer);
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
    rhythm_speed = getGrid();
    rhythm_can_atk = false;
    rhythm_atk = false;
    rhythm_atk_counter = 0;
    rhythm_atk_frame_max = game_fps / 5;
    rhythm_atk_grid = 96;   // 1.5 times the size of a normal grid

    initTexture(rhythm_path, renderer);
}

void Player::stopDash()
{
    on_dash = false;
    on_dash_delay = false;
    dash_counter = 0;
}

void Player::resetMoves()
{
    can_dash = true;
    jump_count = 2;
}

void Player::platformerMvtAccel(Input *input, float dt)
{
    if (!on_wall && !on_dash && !on_wall_jump)
    {
        // Accel
        // Left
        if (input->getPress(Action::MOVE_LEFT) && getVelX() > -vel_x_max)
        {
            right = false;
            setVelX(getVelX() - getAccelX() * dt * 0.5f);
        }

        if (input->getPress(Action::MOVE_RIGHT) && getVelX() < vel_x_max)
        {
            right = true;
            setVelX(getVelX() + getAccelX() * dt * 0.5f);
        }

        // Decel
        if (getVelX() &&                                    // Speed != 0
            input->getPress(Action::MOVE_LEFT) && input->getPress(Action::MOVE_RIGHT) ||     // Dual input
            !input->getPress(Action::MOVE_LEFT) && !input->getPress(Action::MOVE_RIGHT) ||   // No input
            input->getPress(Action::MOVE_LEFT) && getVelX() > 0 ||          // Go left when speed > 0
            input->getPress(Action::MOVE_RIGHT) && getVelX() < 0)            // Go right when speed < 0
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
    }
}

// Player platformer movement
void Player::platformerMvt(Input *input, float dt)
{
    // Accel & decel are performed before and after setX()
    // to avoid difference in distance travelled in different frame rate

    // Speed check are performed before setVel,
    // making it impossible to break the speed limit, same goes for decel

    // 1st half
    platformerMvtAccel(input, dt);    

    // Horizontal movement calculation
    setX(getX() + getVelX() * dt);  
    
    // 2nd half
    platformerMvtAccel(input, dt);

    // Speed cap
    if (getVelX() < -vel_x_max && !on_wall_jump) setVelX(-vel_x_max);
    if (getVelX() > vel_x_max && !on_wall_jump) setVelX(vel_x_max);
    
    // Reset stuff while on the ground and wall
    if (on_ground || on_wall) 
    {
        resetMoves();
    }

    // Wall climb input handler
    if (!climb_up && !climb_down) {
        // climb_stamina = 100
    }

    climb_up = false;
    climb_down = false;
    if (on_wall)
    {
        // climb_stamina --;
        if (input->getPress(Action::MOVE_UP) && !input->getPress(Action::MOVE_DOWN))
        {
            // Vertical movement calculation
            setY(getY() + wall_climb_speed * dt);
            climb_up = true;
        }
        if (input->getPress(Action::MOVE_DOWN) && !input->getPress(Action::MOVE_UP))
        {
            // Vertical movement calculation
            setY(getY() - wall_climb_speed * dt);
            climb_down = true;
        }
    }

    // Jump / double jump / wall jump
    if (input->getPress(Action::ACTION1) && jump_count > 0)
    {
        input->setHold(Action::ACTION1, false);
        jump_start = true;
        jump = true;
    }
    if (jump == true && jump_start == false) 
    {
        if (!on_wall)
        {
            switch (jump_count)
            {
                case 2:
                    setVelY(getGrid() * 12);
                    on_moving_block = false;
                    on_ground = false;
                break;
                case 1:
                    setVelY(getGrid() * 9);
                break;
                default:
                break;
            }
            jump_count--;
        }
        else 
        {
            right = !right;
            setX(getX() + (right ? 1 : -1) * 50);
            setVelX((right ? 1 : -1) * getGrid() * 5);
            setVelY(getGrid() * 10);
            on_wall = false;
            on_wall_jump = true;
            jump_count--;
        }
        jump = false;
    }
    if (on_wall_jump) wall_jump_counter++;
    if (wall_jump_counter >= wall_jump_frame_max) 
    {
        wall_jump_counter = 0;
        on_wall_jump = false;
    }

    // Movement test
    // Aerial velocity control
    if (getVelY() < vel_terminal)
    {
        setVelY(vel_terminal);
    }
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
    if (input->getPress(Action::ACTION2) && can_dash && !on_dash && !on_wall)
    {
        input->setHold(Action::ACTION2, false);
        can_dash = false;
        on_dash = true;
    }
    if (on_dash)
    {
        dash_counter++;

        if (dash_counter < dash_frame_max) setVelX((right ? 1 : -1) * getGrid() * 15);
        else if (dash_counter == dash_frame_max)
        {
            on_dash = false;
            on_dash_delay = true;
            setVelX((right ? 1 : -1) * vel_x_max);
            dash_halt = true;
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

// Player shooter movement
void Player::shooterMvtAccel(Input *input, float dt)
{
    // Up
    if (input->getPress(Action::MOVE_UP) && getVelY() < vel_y_max)
    {
        setVelY(getVelY() + getAccelX() * dt * 0.5f);
    }
    // Down
    if (input->getPress(Action::MOVE_DOWN) && getVelY() > -vel_y_max)
    {
        setVelY(getVelY() - getAccelX() * dt * 0.5f);
    }
    // Left
    if (input->getPress(Action::MOVE_LEFT) && getVelX() > -vel_x_max)
    {
        setVelX(getVelX() - getAccelX() * dt * 0.5f);
    }
    // Right
    if (input->getPress(Action::MOVE_RIGHT) && getVelX() < vel_x_max)
    {
        setVelX(getVelX() + getAccelX() * dt * 0.5f);
    }
    // Decel (First half)
    // Vertical
    if (getVelY() &&                                    // Speed != 0
        input->getPress(Action::MOVE_UP) && input->getPress(Action::MOVE_DOWN) ||     // Dual input
        !input->getPress(Action::MOVE_UP) && !input->getPress(Action::MOVE_DOWN) ||   // No input
        input->getPress(Action::MOVE_UP) && getVelY() < 0 ||          // Go up when speed < 0
        input->getPress(Action::MOVE_DOWN) && getVelY() > 0)            // Go down when speed > 0
    {
        // Decel works by applying 3 times the speed of the opposite direction
        // Down
        if (getVelY() < 0)
        {
            temp_speed_up = getVelY() + getAccelX() * dt * 0.5f * 3;
            setVelY(temp_speed_up < 0 ? temp_speed_up : 0);
        }
        // Up
        else if (getVelY() > 0)
        {
            temp_speed_down = getVelY() - getAccelX() * dt * 0.5f * 3;
            setVelY(temp_speed_down > 0 ? temp_speed_down : 0);
        }
    }
    // Horizontal
    if (getVelX() &&                                    // Speed != 0
        input->getPress(Action::MOVE_LEFT) && input->getPress(Action::MOVE_RIGHT) ||     // Dual input
        !input->getPress(Action::MOVE_LEFT) && !input->getPress(Action::MOVE_RIGHT) ||   // No input
        input->getPress(Action::MOVE_LEFT) && getVelX() > 0 ||          // Go left when speed > 0
        input->getPress(Action::MOVE_RIGHT) && getVelX() < 0)            // Go right when speed < 0
    {
        // Decel works by applying 3 times the speed of the opposite direction
        // Left
        if (getVelX() < 0)
        {
            temp_speed_right = getVelX() + getAccelX() * dt * 0.5f * 3;
            setVelX(temp_speed_right < 0 ? temp_speed_right : 0);
        }
        // Right
        else if (getVelX() > 0)
        {
            temp_speed_left = getVelX() - getAccelX() * dt * 0.5f * 3;
            setVelX(temp_speed_left > 0 ? temp_speed_left : 0);
        }
    }
}

// Same physics
void Player::shooterMvt(Input *input, float dt, uint16_t game_width, uint16_t game_height)
{
    // Accel (First half)
    shooterMvtAccel(input, dt);

    // Movement calculation
    setX(getX() + getVelX() * dt);
    setY(getY() + getVelY() * dt);
    
    // Accel (Second half)
    shooterMvtAccel(input, dt);

    // Prevent player from going out of bound
    if (getX() < 0) setX(0);
    if (getY() < 0) setY(0);
    if (getX() + getWidth() > game_width) setX(game_width - getWidth());
    if (getY() + getHeight() > game_height) setY(game_height - getHeight());

    // Assign hitbox coordinates for collision checks
    smallbox_x[0] = getX() + getGrid() / 48;
    smallbox_y[0] = getY() + getGrid() * 13/48;
    smallbox_x[1] = getX() + getGrid() * 5/32;
    smallbox_y[1] = getY() + getGrid() * 5/96;
    smallbox_x[2] = getX() + getGrid() * 3/8;
    smallbox_y[2] = getY() + getGrid() * 13/48;
}

// Different attack style
void Player::shooterVertAtk(Scene *scene, Input *input, float dt)
{
    // Allows for hold, no need to spam attack to shoot
    // An interval of half a second for default projectile speed
    // and a quarter of a second for stage 1 and beyond
    if (input->getPress(Action::ACTION1) && shooter_can_atk)
    {
        projectiles.push_back(new Projectile(getX() + getWidth() + getGrid() / 4, getY(), "res/Character Sheets/Drool.png"));
        projectiles.back()->initStraightProj(scene->getRenderer(), false);
        shooter_can_atk = false;
    }
    if (!shooter_can_atk)
    {
        shooter_atk_counter++;
        if (shooter_atk_counter >= shooter_atk_delay) shooter_can_atk = true;
    }
}

void Player::shooterHoriAtk(Scene *scene, Input *input, float dt)
{
    // Allows for hold, no need to spam attack to shoot
    // An interval of half one third of a second for projectile
    if (shooter_can_atk)
    {
        if (input->getPress(Action::ACTION1))
        {
            // Projectile spawn coordinates tbd, as the sprite isn't done yet
            projectiles.push_back(new Projectile(getX() + getWidth(), getY(), "res/Character Sheets/Drool.png"));
            projectiles.back()->initStraightProj(scene->getRenderer(), false);
            shooter_can_atk = false;
        }
    }
    else
    {
        shooter_atk_counter++;
        if (shooter_atk_counter >= shooter_atk_delay) 
        {
            shooter_can_atk = true;
            shooter_atk_counter = 0;
        }
    }

    for (int i = 0; i < projectiles.size(); i++)
    {
        projectiles[i]->projectileMovement(dt);
        
        if (projectiles[i]->getX() > scene->getWidth()) 
        {
            delete projectiles[i];
            projectiles[i] = nullptr;
            projectiles.erase(projectiles.begin() + i);
        } 
    }

    projectiles.shrink_to_fit();

    // std::cout << projectiles.size() << "\n";
}

// Player rhythm movement
void Player::rhythmMvt(Input *input, float dt)
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
    if (input->getPress(Action::MOVE_UP) && current_lane < 3)
    {
        input->setHold(Action::MOVE_UP, false);
        setX(getX() + getGrid());
        setY(getY() + getGrid() * 3);
        current_lane++;
    }
    if (input->getPress(Action::MOVE_DOWN) && current_lane > 1)
    {
        input->setHold(Action::MOVE_DOWN, false);
        setX(getX() - getGrid());
        setY(getY() - getGrid() * 3);
        current_lane--;
    }

    // Horizontal movement
    // Left and right movement
    if (!on_dash && input->getPress(Action::MOVE_LEFT))
    {
        setVelX(getVelX() - getGrid() * 2);
    }
    if (!on_dash && input->getPress(Action::MOVE_RIGHT))
    {
        setVelX(getVelX() + getGrid() * 2);
    }
    // Dash
    if (input->getPress(Action::ACTION2) && can_dash && !on_dash)
    {
        input->setHold(Action::ACTION2, false);
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
    setX(getX() + rhythm_speed * dt);  
    std::cout << getX() + rhythm_speed * dt << " ";

    // Vertical movement + gravity
    // Jump
    if (input->getPress(Action::ACTION1))
    {
        input->setHold(Action::ACTION1, false);
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
    if (input->getPress(Action::ACTION1) && rhythm_can_atk)
    {
        input->setHold(Action::ACTION1, false);
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

Player::~Player()
{ 
    std::cout << "Player terminated!\n";
}