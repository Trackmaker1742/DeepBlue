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
    void playerBlockColli(Player* player, std::vector<Block*> Blocks);
    void playerPrev(Player* player);
};

#endif