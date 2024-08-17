#include "collision.h"

void Collision::playerBlockColli(Stage* stage, Player* player, std::vector<Block*> Blocks)
{
    spring_up_vel_y = player->getGrid() * 15;
    spring_down_vel_y = -player->getGrid() * 15;
    spring_left_vel_x = -player->getGrid() * 20;
    spring_right_vel_x = player->getGrid() * 20;
    spring_hori_vel_y = player->getGrid() * 13;

    // Boundary (bottom and left)
    if (player->getX() < 0 || player->getY() < 0)
    {
        // Player respawn when out of bound
        player->setX(stage->getRespX());
        player->setY(stage->getRespY());
    }

    // Blocks not affecting movements (items, checkpoints,...)
    for (Block* b : Blocks)
    {
        if (b->getType() > 5) continue;
        // If player hitbox overlaps with block hitbox
        if (player->getX() + player->getWidth() > b->getGridX() &&
        player->getX() < b->getGridX() + b->getGrid() &&
        player->getY() + player->getHeight() > b->getGridY() &&
        player->getY() < b->getGridY() + b->getGrid())
        {
            // Gotta work on respawn points
            // Danger blocks kill on hit :33
            switch (b->getType())
            {
                case 2: // Checkpoint block
                    stage->setRespX(b->getGridX());
                    stage->setRespY(b->getGridY());
                break;
                case 3: // Goal block
                    
                break;
                case 4: // Danger block
                    player->setX(stage->getRespX());
                    player->setY(stage->getRespY());
                break;
                case 5: // Item block
                        // Handle the item buffs from here seems like the fastest option
                        // Nah, I think I should handle it from player, and have this as a trigger for the buff
                break;
            }
        }
    }

    // Blocks affecting movement (normal block, slope,...)
    player->setOnGround(false);
    // Bottom of player
    for (Block* b : Blocks)
    {
        if (b->getType() < 6) continue;
        // Normal square block, wall-grab block, moving platform
        if (b->getType() != 7 && b->getType() != 8 &&
        player_prev_x < b->getGridX() + b->getGrid() && 
        player->getX() < b->getGridX() + b->getGrid() && 
        player_prev_x + player->getWidth() > b->getGridX() && 
        player->getX() + player->getWidth() > b->getGridX())
        {
            if (player->getY() > b->getGridY() && 
            player->getY() <= b->getGridY() + b->getGrid())
            {
                // Spring (up)
                if (b->getType() == 10)
                {
                    player->setVelY(spring_up_vel_y);
                    break;
                }
                // Spring (left) side collision
                if (b->getType() == 12)
                {
                    player->setVelX(spring_left_vel_x);
                    player->setVelY(spring_hori_vel_y);
                    break;
                }
                // Spring (right) side collision
                if (b->getType() == 13)
                {
                    player->setVelX(spring_right_vel_x);
                    player->setVelY(spring_hori_vel_y);
                    break;
                }

                // Normal blocks
                player->setY(b->getGridY() + b->getGrid());
                player->setOnGround(true);
                continue;
            }
        }

        // Slope
        if (player->getX() + player->getWidth() > b->getGridX() &&
        player->getX() < b->getGridX() + b->getGrid())
        {
            // Left slope
            if (b->getType() == 7)
            {
                if (player->getX() + player->getWidth() <= b->getGridX() + b->getGrid() && 
                player->getY() <= player->getX() + player->getWidth() - b->getGridX() + b->getGridY() + 1 &&
                player->getY() >= b->getGridY())
                {
                    player->setY(player->getX() + player->getWidth() - b->getGridX() + b->getGridY() + 1);
                    player->setOnGround(true);
                    break;
                }
                else if (player->getX() + player->getWidth() >= b->getGridX() + b->getGrid() && 
                player->getY() <= b->getGridY() + b->getGrid() &&
                player->getY() >= b->getGridY() + b->getGrid() / 2)
                {
                    player->setY(b->getGridY() + b->getGrid());
                    player->setOnGround(true);
                    continue;
                }
            }
            // Right slope
            if (b->getType() == 8)
            {
                if (player->getX() >= b->getGridX() && 
                player->getY() <= b->getGrid() - player->getX() + b->getGridX() + b->getGridY() + 1 &&
                player->getY() >= b->getGridY())
                {
                    player->setY(b->getGrid() - player->getX() + b->getGridX() + b->getGridY() + 1);
                    player->setOnGround(true);
                    break;
                }
                else if (player->getX() <= b->getGridX() && 
                player->getY() <= b->getGridY() + b->getGrid() &&
                player->getY() >= b->getGridY() + b->getGrid() / 2)
                {
                    player->setY(b->getGridY() + b->getGrid());
                    player->setOnGround(true);
                    continue;
                }
            }
        }
    }
    
    player->setOnWall(false);
    // Left, Right, Top of player
    for (Block* b : Blocks)
    {
        if (b->getType() < 6) continue;
        // Left and Right collision 
        // Due to some condition overlaps, left and right collision of slope doesn't work
        if (player_prev_y < b->getGridY() + b->getGrid() && 
        player->getY() < b->getGridY() + b->getGrid() && 
        player_prev_y + player->getHeight() > b->getGridY() && 
        player->getY() + player->getHeight() > b->getGridY())
        {
            // Left
            if (player->getX() > b->getGridX() && 
            player->getX() < b->getGridX() + b->getGrid())
            {
                // Normal, moving platform block
                if (b->getType() == 6)
                {
                    player->setX(b->getGridX() + b->getGrid());
                    player->setVelX(-player->getVelX() * 0.3);
                    continue;
                }
                // Left slope
                else if (b->getType() == 7)
                {
                    if (player_prev_x > b->getGridX() + b->getGrid())
                    {
                        player->setX(b->getGridX() + b->getGrid() + 1);
                        player->setVelX(-player->getVelX() * 0.3);
                        continue;
                    }
                }
                // Wall grab block collision
                else if (b->getType() == 9)
                {
                    player->setOnWall(true);
                    player->setRight(false);
                    player->setX(b->getGridX() + b->getGrid());
                    continue;
                }
                // Spring (up) side collision
                else if (b->getType() == 10)
                {
                    player->setY(player->getY() + player->getGrid() / 20);
                    player->setVelY(spring_up_vel_y);
                    break;
                }
                // Spring (down)
                else if (b->getType() == 11)
                {
                    player->setVelY(spring_down_vel_y);
                    break;
                }
                // Spring (up-left) side collision
                else if (b->getType() == 12)
                {
                    player->setVelX(spring_left_vel_x);
                    player->setVelY(spring_hori_vel_y);
                    break;
                }
            }
            // Right
            if (player->getX() < b->getGridX() && 
            player->getX() + player->getWidth() > b->getGridX())
            {
                // Normal, moving platform block
                if (b->getType() == 6)
                {
                    player->setX(b->getGridX() - player->getWidth());
                    player->setVelX(-player->getVelX() * 0.3);
                    continue;
                }            
                // Right slope
                else if (b->getType() == 8)
                {
                    if (player_prev_x + player->getWidth() < b->getGridX())
                    {
                        player->setX(b->getGridX() - player->getWidth() - 1);
                        player->setVelX(-player->getVelX() * 0.3);
                        continue;
                    }
                }
                // Wall grab block collision
                else if (b->getType() == 9)
                {
                    player->setOnWall(true);
                    player->setRight(true);
                    player->setX(b->getGridX() - player->getWidth());
                    continue;
                }
                // Spring (up) side collision
                else if (b->getType() == 10)
                {
                    player->setY(player->getY() + player->getGrid() / 20);
                    player->setVelY(spring_up_vel_y);
                    break;
                }
                // Spring (down)
                else if (b->getType() == 11)
                {
                    player->setVelY(spring_down_vel_y);
                    break;
                }
                // Spring (up-right) side collision
                else if (b->getType() == 13)
                {
                    player->setVelX(spring_right_vel_x);
                    player->setVelY(spring_hori_vel_y);
                    break;
                }
            }
        }

        // Top collision (hitting ceiling)
        if (player_prev_x < b->getGridX() + b->getGrid() && 
        player->getX() < b->getGridX() + b->getGrid() && 
        player_prev_x + player->getWidth() > b->getGridX() && 
        player->getX() + player->getWidth() > b->getGridX())
        {
            if (player->getY() < b->getGridY() && 
            player->getY() + player->getHeight() > b->getGridY())
            {
                // Spring (down)
                if (b->getType() == 11)
                {
                    player->setVelY(spring_down_vel_y);
                    break;
                }
                // Spring (up-left) side collision
                else if (b->getType() == 12)
                {
                    player->setVelX(spring_left_vel_x);
                    player->setVelY(spring_hori_vel_y);
                    break;
                }
                // Spring (up-right) side collision
                else if (b->getType() == 13)
                {
                    player->setVelX(spring_right_vel_x);
                    player->setVelY(spring_hori_vel_y);
                    break;
                }
                player->setY(b->getGridY() - player->getHeight());
                player->setVelY(-player->getVelY() * 0.1);
                continue;
            }
        }
    }

    // Ascend, descend check
    if (player->getVelY() != 0)
    {
        if (player->getY() > player_prev_y && 
        !player->getJumpStart() && !player->getApex() && !player->getLand())
        {
            player->setAscend(true);
            player->setDescend(false);
        }
        else if (player->getY() < player_prev_y && 
        !player->getJumpStart() && !player->getApex() && !player->getLand())
        {
            player->setAscend(false);
            player->setDescend(true);
        }
        // If vel y suddenly shifts from + to -
        // The result of gravity on ascend velocity
        if (player_prev_vel_y > 0 && player->getVelY() < 0)
        {
            player->setApex(true);
            player->setAscend(false);
            player->setDescend(false);
        }
    }
    else
    {
        player->setAscend(false);
        player->setDescend(false);
    }
    // Landing trigger, a shift from not on ground to on ground
    if (player->getOnGround() && player_prev_on_ground == false)
    {
        player->setLand(true);
        player->setAscend(false);
        player->setDescend(false);
    }

    // Get previous position
    player_prev_x = player->getX();
    player_prev_y = player->getY();
    // Get previous y velocity
    player_prev_vel_y = player->getVelY();
    player_prev_on_ground = player->getOnGround();
}

void Collision::playerRhythmColli(Stage *stage, Player *player, std::vector<Block*> Blocks)
{
    // Get lane, then proceed to deal with collision on each lane
    // New csv reading method required
    // No need to worry about ground collision

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

    // Object collision
    for (Block *b : Blocks)
    {
        // If player, block are on the same lane
        if (player->getLane() == b->getLane())
        {
            // And collide, only check if not invul
            if (!player->getInvul() && 
            player->getX() + player->getGrid() > b->getGridX() &&
            player->getX() < b->getGridX() + b->getGrid() &&
            player->getY() + player->getGrid() > b->getGridY() &&
            player->getY() < b->getGridY() + b->getGrid())
            {
                // Respawn on hit, will implement 
                // a black screen, music reset and stuff later
                player->setRhyBar(player->getRhyBar() - 30);
            }

            // Collision with attack hurtbox, works even when invul
            // If currently attacking, and hitting breakable block
            // Then overlapping hitbox, the usual
            if (player->getRhyAtk() && b->getType() == 4 &&
            player->getRhyAtkX() + player->getRhyAtkGrid() > b->getGridX() &&
            player->getRhyAtkX() < b->getGridX() + b->getGrid() &&
            player->getRhyAtkY() + player->getRhyAtkGrid() > b->getGridY() &&
            player->getRhyAtkY() < b->getGridY() + b->getGrid())
            {
                
            }
        }
    }
}

// Shooter stuff
void Collision::playerEnemyColli(Enemy *enemy, Player *player)
{
    // Every object is a square now with how I use grid collision lol
    if (player->getX() < enemy->getX() + enemy->getGrid() &&
    player->getX() + player->getX() > enemy->getX() &&
    player->getY() < enemy->getY() + enemy->getGrid() &&
    player->getY() + player->getGrid() > enemy->getY())
    {
        // Do something when player collide with enemy
    }
}
void Collision::projectilePlayerColli(Projectile *proj, Player *player)
{
    if (player->getX() < proj->getX() + proj->getGrid() &&
    player->getX() + player->getX() > proj->getX() &&
    player->getY() < proj->getY() + proj->getGrid() &&
    player->getY() + player->getGrid() > proj->getY())
    {
        // Do something when player collide with proj
    }
}
void Collision::projectileEnemyColli(Projectile *proj, Enemy *enemy)
{
    if (enemy->getX() < proj->getX() + proj->getGrid() &&
    enemy->getX() + enemy->getX() > proj->getX() &&
    enemy->getY() < proj->getY() + proj->getGrid() &&
    enemy->getY() + enemy->getGrid() > proj->getY())
    {
        // Do something when enemy collide with proj
    }
}

Collision::~Collision()
{ }