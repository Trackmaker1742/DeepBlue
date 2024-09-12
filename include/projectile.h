#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <math.h>

#include "object2d.h"

class Projectile : public Object2d
{
private:
    uint8_t damage_value = 1;
    bool light;  // Dark and light bullet

    // For player
    bool vertical = true;

    // Boolean to identify player or enemy projectile
    bool from_enemy = 0;

    bool aim = false;

    // Center position for more accurate tracking
    float center_start_x;
    float center_start_y;
    float player_center_x;
    float player_center_y;

    // Distance for projectile vector calculation
    float dist_x;
    float dist_y;
    float dist;

    // Enhanced projectile state, for bosses
    // Same damage value for all bullet type
    // Player buff will be in the form of more bullet
    bool size_amp = false;      // Size: 32x32

public:
    Projectile(float X, float Y, const char *p, bool fe = 0, bool size = 0);
    Projectile(float X, float Y, bool l, const char *p, bool fe = 0, bool size = 0);

    bool getLight();

    // Init, reinit bullet properties
    void initStraightProj(SDL_Renderer *renderer, bool v);
    // Enemy bullet consist of straight bullet, aiming bullet (no homing)
    // Aiming bullet will target the position of player at the point the bullet is shot
    // and it cannot track the player as they move
    void initAimingProj(SDL_Renderer *renderer, float start_x, float start_y, float end_x, float end_y);
    
    // Surprisingly easy movement function
    void projectileMovement(float dt);

    ~Projectile();
};  

#endif