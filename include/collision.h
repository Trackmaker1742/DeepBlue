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
public:
    void playerBlockColli(Stage *stage, Player *player, std::vector<Block*> Blocks);
    void playerPrev(Player *player);

    // Rhythm stuff
    void playerObsColli(Stage *stage, Player *player, std::vector<Block*> Blocks);

    // Shooter stuff
    void playerEnemyColli();
    void playerProjectileColli();
    void projectileEnemyColli();

    ~Collision();
};

#endif