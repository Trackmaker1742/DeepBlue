#include "projectile.h"

Projectile::Projectile(float X, float Y, const char *p, bool size, bool fe) : Object2d(X, Y, p), from_enemy(fe), size_amp(size)
{ }

void Projectile::initStraightProj(SDL_Renderer *renderer, bool v)
{
    // Set direction of bullet (to separate 2 game mode)
    vertical = v;
    // Set speed for each mode
    // Player projectiles only go up and right
    // Enemy projectiles only go down and left
    // This patchwork solution should suffice
    if (vertical)
    {   
        setVelY(getGrid() * (from_enemy ? -4 : 20));
        setVelX(0);
    }
    else
    {   
        setVelX(getGrid() * (from_enemy ? -4 : 20));
        setVelY(0);
    }

    // Projectile texture
    initTexture(renderer);
}

void Projectile::initAimingProj(SDL_Renderer *renderer, float start_x, float start_y, float end_x, float end_y)
{
    // Set center based on projectile size
    if (!size_amp)
    {
        center_start_x = start_x + 8;
        center_start_y = start_y + 8;
    }
    else 
    {
        center_start_x = start_x + 16;
        center_start_y = start_y + 16;
    }

    // Target player center
    player_center_x = end_x + 32;
    player_center_y = end_y + 32;

    // Get distance to calculate vector
    dist_x = player_center_x - center_start_x;
    dist_y = player_center_y - center_start_y;
    dist = sqrtf(dist_x * dist_x + dist_y * dist_y);

    // Set velocity
    setVelX(getGrid() * 2);
    setVelY(getGrid() * 2);

    // Projectile texture 
    initTexture(renderer);
}

void Projectile::projectileMovement(float dt)
{
    // Simple movement, unchanged speed
    if (aim)
    {
        setX(getX() + (dist_x / dist) * getVelX() * dt);
        setY(getY() + (dist_y / dist) * getVelY() * dt);
    }
    else 
    {
        setX(getX() + getVelX() * dt);
        setY(getY() + getVelY() * dt);
    }
}

Projectile::~Projectile()
{ 
    // std::cout << "destroyed\n";
}