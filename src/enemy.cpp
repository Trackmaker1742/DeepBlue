#include "enemy.h"

Enemy::Enemy(uint8_t fps, float X, float Y, const char *P, uint8_t T) : 
    Object2d(X, Y, P), game_fps(fps), type(T)
{ }

void Enemy::init()
{
    attack_counter = 0;
    // Init will adjust based on the enemy type
    // Grid size should be the same for all of the mob enemies (64)
    switch (type)
    {
        case 1:
            health = 50;
            attack_cooldown = game_fps;
        break;
    }
}

Enemy::~Enemy() 
{ }