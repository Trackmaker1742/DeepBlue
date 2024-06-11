#include "collision.h"

void Collision::playerBlockColli(Player* player, std::vector<Block*> Blocks)
{
    // Bottom 
    for (Block* b : Blocks)
    {
        if (player_prev_x < b->getGridX() + b->getGrid() && player->getX() < b->getGridX() + b->getGrid() && player_prev_x + player->getGrid() > b->getGridX() && player->getX() + player->getGrid() > b->getGridX())
        {
            if (player->getY() > b->getGridY() && player->getY() < b->getGridY() + b->getGrid())
            {
                player->setY(b->getGridY() + b->getGrid());
                player->setOnGround(true);
                break;
            }
        }
        player->setOnGround(false);
    }
    // Left, Right, Top
    for (Block* b : Blocks)
    {
        // Left and Right collision
        if (player_prev_y < b->getGridY() + b->getGrid() && player->getY() < b->getGridY() + b->getGrid() && player_prev_y + player->getGrid() > b->getGridY() && player->getY() + player->getGrid() > b->getGridY())
        {
            // Left
            if (player->getX() < b->getGridX() && player->getX() + player->getGrid() > b->getGridX())
            {
                player->setX(b->getGridX() - player->getGrid());
                player->setVelX(-player->getVelX() * 0.5);
                continue;
            }
            // Right
            if (player->getX() > b->getGridX() && player->getX() < b->getGridX() + b->getGrid())
            {
                player->setX(b->getGridX() + b->getGrid());
                player->setVelX(-player->getVelX() * 0.5);
                continue;
            }
        }

        // Top collision (hitting ceiling)
        if (player_prev_x < b->getGridX() + b->getGrid() && player->getX() < b->getGridX() + b->getGrid() && player_prev_x + player->getGrid() > b->getGridX() && player->getX() + player->getGrid() > b->getGridX())
        {
            if (player->getY() < b->getGridY() && player->getY() + player->getGrid() > b->getGridY())
            {
                player->setY(b->getGridY() - player->getGrid());
                continue;
            }
        }
    }
}

void Collision::playerPrev(Player *player)
{
    // Get previous position
    player_prev_x = player->getX();
    player_prev_y = player->getY();
}