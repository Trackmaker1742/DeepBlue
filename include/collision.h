#ifndef COLLISION_H
#define COLLISION_H

#include <iostream>
#include <math.h>
#include <vector>

#include "stage.h"
#include "player.h"

class Collision 
{
private:
    float player_prev_x;
    float player_prev_y;

    float player_prev_vel_y;
    bool player_prev_on_ground;

    float spring_up_vel_y;
    float spring_down_vel_y;
    float spring_left_vel_x;
    float spring_right_vel_x;
    float spring_hori_vel_y;
public:
    // Non physical blocks (from 1 to 5, 0 is empty void)
    // Allowing vector input because danger blocks can move
    void specialBlockColli(Stage *stage, std::vector<Block*> Blocks, Player *player);
    // Normal colllsion for normal blocks (from 6 to 9)
    void blockTopColli(std::vector<Block*> Blocks, Player *player, float dt);
    void blockBotSideColli(std::vector<Block*> Blocks, Player *player, float dt);
    // Spring colli handled separately to keep the code compact (from 10 to 13)
    void springBlockColli(std::vector<Block*> Blocks, Player *player);

    // Platformer collision
    void playerBlockColli(Stage *stage, Player *player, float dt);

    // Rhythm stuff
    void playerRhythmColli(Stage *stage, Player *player);

    // Shooter stuff
    void playerEnemyColli(Enemy *enemy, Player *player);
    void projectilePlayerColli(Projectile *proj, Player *player);
    void projectileEnemyColli(Projectile *proj, Enemy *enemy);

    ~Collision();
};

#endif