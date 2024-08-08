#ifndef ENEMY_H
#define ENEMY_H

#include "object2d.h"

class Enemy : public Object2d
{
private:
    // Enemy hp, will be tanking a bunch of fast bullet with 1 point of dmg
    // uint16_t, so 65k is the max health
    uint16_t health;

    // Enemy type, to deal with attack and movement pattern for each enemy
    uint8_t type;

    // Rate of fire of enemy, tougher enemy => higher firing rate
    uint8_t attack_counter;
    float attack_cooldown;  // Based on game_fps

    // Fps for time related calculations
    float game_fps;

public:
    Enemy(uint8_t fps, float X, float Y, const char *P, uint8_t T);

    void init();    // Type of enemy:
                    // 0: Move to position -> stationary
                    // 1: Move to position -> move around preset coordinates
                    // 2: Move to position -> track the player's x
                    // 3: Move to position -> track the player's y
                    // 4: Move to position -> track the player up to a certain point (suicide bomber)
                    // 5: Boss enemy (stage 5)


    ~Enemy();
};

#endif