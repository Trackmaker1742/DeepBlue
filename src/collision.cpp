#include "collision.h"

void Collision::specialBlockColli(Stage *stage, std::vector<Block*> Blocks, Player *player)
{   
    for (Block *b : Blocks)
    {
        // Skip blocks outside the screen
        if (!b->getOnScreen()) continue;
        
        if (b->getType() > 5) continue;
        // If player hitbox overlaps with block hitbox
        if (player->getX() + player->getWidth() > b->getX() &&
        player->getX() < b->getX() + b->getGrid() &&
        player->getY() + player->getHeight() > b->getY() &&
        player->getY() < b->getY() + b->getGrid())
        {
            // Gotta work on respawn points
            // Danger blocks kill on hit :33
            switch (b->getType())
            {
                case 2: // Checkpoint block
                    stage->setRespX(b->getX());
                    stage->setRespY(b->getY());
                break;
                case 3: // Goal block
                    
                break;
                case 4: // Danger block
                    player->setX(stage->getRespX());
                    player->setY(stage->getRespY());
                    player->resetMoves();
                break;
                case 5: // Item block
                        // Handle the item buffs from here seems like the fastest option
                        // Nah, I think I should handle it from player, and have this as a trigger for the buff
                break;
            }
        }
    }
}

void Collision::blockTopColli(std::vector<Block*> Blocks, Player *player, float dt)
{
    // Top of blocks
    for (Block *b : Blocks)
    {
        // Skip blocks outside the screen
        if (!b->getOnScreen()) continue;

        // Skip over other wall block ground collision
        // For chunks of moving wall blocks
        if (b->getType() == 11 && player->getOnWall()) continue;
        // Skip non-physics blocks, destination blocks
        if (b->getType() < 6 || b->getType() > 12) continue;
        if (player_prev_x < b->getX() + b->getGrid() && 
        player->getX() < b->getX() + b->getGrid() && 
        player_prev_x + player->getWidth() > b->getX() && 
        player->getX() + player->getWidth() > b->getX())
        {
            // Not slope (can move)
            if (b->getType() != 7 && b->getType() != 8)
            {
                if (player->getY() > b->getY() && 
                player->getY() <= b->getY() + b->getGrid() && 
                player_prev_y >= player->getY())
                {
                    // Moving blocks
                    if (b->getVelX()) 
                    {
                        player->setX(player->getX() + b->getVelX() * dt);
                    }
                    if (b->getVelY())
                    {
                        b->setStoodOn(true);
                        player->setOnMovingBlock(true);
                    }

                    player->setY(b->getY() + b->getGrid());
                    player->setOnGround(true);
                    continue;
                }
                if (player->getOnMovingBlock() == true && 
                b->getStoodOn() == true)
                {
                    player->setY(b->getY() + b->getGrid());
                }
            }
            // Up left slope
            else if (b->getType() == 7)
            {
                if (player->getX() + player->getWidth() <= b->getX() + b->getGrid() && 
                player->getY() <= b->getY() + (player->getX() + player->getWidth() - b->getX()) + 1 &&
                player->getY() >= b->getY())
                {
                    player->setY(b->getY() + (player->getX() + player->getWidth() - b->getX()) + 1);
                    player->setOnGround(true);
                    break;
                }
                else if (player->getX() + player->getWidth() >= b->getX() + b->getGrid() && 
                player->getY() <= b->getY() + b->getGrid() &&
                player->getY() >= b->getY() + b->getGrid() / 2)
                {
                    player->setY(b->getY() + b->getGrid());
                    player->setOnGround(true);
                    continue;
                }
            }
            // Up right slope
            else if (b->getType() == 8)
            {
                if (player->getX() >= b->getX() && 
                player->getY() <= b->getY() + (b->getGrid() - player->getX() + b->getX()) + 1 &&
                player->getY() >= b->getY())
                {
                    player->setY(b->getY() + (b->getGrid() - player->getX() + b->getX()) + 1);
                    player->setOnGround(true);
                    break;
                }
                else if (player->getX() <= b->getX() && 
                player->getY() <= b->getY() + b->getGrid() &&
                player->getY() >= b->getY() + b->getGrid() / 2)
                {
                    player->setY(b->getY() + b->getGrid());
                    player->setOnGround(true);
                    continue;
                }
            }
        }
        else if ((player_prev_x >= b->getX() + b->getGrid() && 
        player->getX() >= b->getX() + b->getGrid()) || 
        (player_prev_x + player->getWidth() <= b->getX() && 
        player->getX() + player->getWidth() <= b->getX()))
        {
            b->setStoodOn(false);
        }
    }
}

void Collision::blockBotSideColli(std::vector<Block*> Blocks, Player *player, float dt)
{
    // Right, Left, Bottom of block
    for (Block *b : Blocks)
    {
        // Skip blocks outside the screen
        if (!b->getOnScreen()) continue;
        
        // Skipping bridge block (12)
        if (b->getType() < 6 || b->getType() > 11) continue;
        // Left and Right collision
        if (player_prev_y < b->getY() + b->getGrid() && 
        player->getY() < b->getY() + b->getGrid() && 
        player_prev_y + player->getHeight() > b->getY() && 
        player->getY() + player->getHeight() > b->getY())
        {
            b->setClimbedOn(false);
            // Left side of blocks
            if (player->getX() < b->getX() && 
            player->getX() + player->getWidth() > b->getX())
            {
                // Normal, moving platform block
                if (b->getType() == 6)
                {
                    player->setX(b->getX() - player->getWidth());
                    player->setVelX(-player->getVelX() * 0.3);
                    continue;
                }
                // Up left slope
                else if (b->getType() == 7)
                {
                    if (player->getY() + player->getHeight() * 0.2 < b->getY())
                    {
                        player->setX(b->getX() - player->getWidth() - 1);
                        player->setVelX(-player->getVelX() * 0.3);
                        continue;
                    }
                }
                // Up & down right slope
                else if (b->getType() == 8 || b->getType() == 10)
                {
                    if (player_prev_x + player->getWidth() < b->getX())
                    {
                        player->setX(b->getX() - player->getWidth() - 1);
                        player->setVelX(-player->getVelX() * 0.3);
                        continue;
                    }
                }
                // Wall grab block collision
                else if (b->getType() == 11)
                {
                    player->setOnWall(true);
                    player->setRight(true);
                    player->setX(b->getX() - player->getWidth());
                    b->setClimbedOn(true);
                    // Moving wall blocks
                    if (b->getVelY() && 
                    b->getClimbedOn() == true &&
                    player->getOnWall() == true) 
                    {
                        player->setY(player->getY() + b->getVelY() * dt);
                    }
                    continue;
                }
            }
            // Right side of blocks
            if (player->getX() > b->getX() && 
            player->getX() < b->getX() + b->getGrid())
            {
                // Normal, moving platform block
                if (b->getType() == 6)
                {
                    player->setX(b->getX() + b->getGrid());
                    player->setVelX(-player->getVelX() * 0.3);
                    continue;
                }
                // Up & down left slope
                else if (b->getType() == 7 || b->getType() == 9)
                {
                    if (player_prev_x > b->getX() + b->getGrid())
                    {
                        player->setX(b->getX() + b->getGrid() + 1);
                        player->setVelX(-player->getVelX() * 0.3);
                        continue;
                    }
                }
                // Up right slope
                else if (b->getType() == 8)
                {
                    if (player->getY() + player->getHeight() * 0.2 < b->getY())
                    {
                        player->setX(b->getX() + b->getGrid() + 1);
                        player->setVelX(-player->getVelX() * 0.3);
                        continue;
                    }
                }
                // Wall grab block collision
                else if (b->getType() == 11)
                {
                    player->setOnWall(true);
                    player->setRight(false);
                    player->setX(b->getX() + b->getGrid());
                    b->setClimbedOn(true);
                    // Moving wall blocks
                    if (b->getVelY() && 
                    b->getClimbedOn() == true &&
                    player->getOnWall() == true) 
                    {
                        player->setY(player->getY() + b->getVelY() * dt);
                    }
                    continue;
                }
            }
        }

        // Bottom of blocks
        if (player_prev_x < b->getX() + b->getGrid() && 
        player->getX() < b->getX() + b->getGrid() && 
        player_prev_x + player->getWidth() > b->getX() && 
        player->getX() + player->getWidth() > b->getX())
        {
            // Down left slope
            if (b->getType() == 9)
            {
                if (player->getX() + player->getWidth() <= b->getX() + b->getGrid() && 
                player->getY() >= b->getY() - (player->getX() + player->getWidth() - b->getX()) - 1 &&
                player->getY() + player->getHeight() <= b->getY() + b->getGrid())
                {
                    player->setY(b->getY() - (player->getX() + player->getWidth() - b->getX()) - 1);
                    player->setVelY(0);
                    break;
                }
                else if (player->getX() + player->getWidth() >= b->getX() + b->getGrid() && 
                player->getY() + player->getHeight() >= b->getY() &&
                player->getY() + player->getHeight() <= b->getY() + b->getGrid() / 2)
                {
                    player->setY(b->getY() - player->getHeight());
                    player->setVelY(0);
                    break;
                }
            }
            // Down right slope
            else if (b->getType() == 10)
            {
                if (player->getX() >= b->getX() && 
                player->getY() >= b->getY() - (b->getGrid() - player->getX() + b->getX()) - 1 &&
                player->getY() + player->getHeight() <= b->getY() + b->getGrid())
                {
                    player->setY(b->getY() - (b->getGrid() - player->getX() + b->getX()) - 1);
                    player->setVelY(0);
                    break;
                }
                else if (player->getX() <= b->getX() && 
                player->getY() + player->getHeight() >= b->getY() &&
                player->getY() + player->getHeight() <= b->getY() + b->getGrid() / 2)
                {
                    player->setY(b->getY() - player->getHeight());
                    player->setVelY(0);
                    break;
                }
            }
            else if (player->getY() + player->getHeight() * 0.8 < b->getY() && 
            player->getY() + player->getHeight() > b->getY())
            {
                player->setY(b->getY() - player->getHeight());
                player->setVelY(0);
                break;
            }
        }
    }
}

void Collision::springBlockColli(std::vector<Block*> blocks, Player *player)
{
    for (Block *b : blocks)
    {
        // Skip blocks outside the screen
        if (!b->getOnScreen()) continue;
        
        if (b->getType() < 13) continue;
        if (player->getX() + player->getWidth() > b->getX() &&
        player->getX() < b->getX() + b->getGrid() &&
        player->getY() + player->getHeight() > b->getY() &&
        player->getY() < b->getY() + b->getGrid())
        {
            player->stopDash();
            player->resetMoves();
            player->setY(player->getY() + player->getGrid() / 20);
            switch (b->getType())
            {
                case 13:
                    player->setVelY(spring_up_vel_y);
                    break;
                case 14:
                    player->setVelY(spring_down_vel_y);
                    break;
                case 15:
                    player->setVelX(spring_left_vel_x);
                    player->setVelY(spring_hori_vel_y);
                    break;
                case 16:
                    player->setVelX(spring_right_vel_x);
                    player->setVelY(spring_hori_vel_y);
                    break;
                default:
                break;
            }
        }
    }
}

void Collision::playerBlockColli(Stage* stage, Player* player, float dt)
{
    spring_up_vel_y = player->getGrid() * 15;
    spring_down_vel_y = -player->getGrid() * 15;
    spring_left_vel_x = -player->getGrid() * 20;
    spring_right_vel_x = player->getGrid() * 20;
    spring_hori_vel_y = player->getGrid() * 13;

    // Boundary (bottom)
    if (player->getY() < 0)
    {
        // Player respawn when out of bound
        player->setX(stage->getRespX());
        player->setY(stage->getRespY());
    }

    // Collision check shared with moving block of the same type
    // Blocks not affecting movements (items, checkpoints,...)
    specialBlockColli(stage, stage->getBlockVec(), player);
    specialBlockColli(stage, stage->getMovingBlockVec(), player);
    // Spring blocks collision handler
    springBlockColli(stage->getBlockVec(), player);
    springBlockColli(stage->getMovingBlockVec(), player);
    // Blocks affecting movement (normal block, slope,...)
    player->setOnGround(false);
    blockTopColli(stage->getBlockVec(), player, dt);
    blockTopColli(stage->getMovingBlockVec(), player, dt);
    player->setOnWall(false);
    blockBotSideColli(stage->getBlockVec(), player, dt);
    blockBotSideColli(stage->getMovingBlockVec(), player, dt);
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

void Collision::playerRhythmColli(Stage *stage, Player *player)
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
    for (Block *b : stage->getBlockVec())
    {
        // If player, block are on the same lane
        if (player->getLane() == b->getLane())
        {
            // And collide, only check if not invul
            if (!player->getInvul() && 
            player->getX() + player->getGrid() > b->getX() &&
            player->getX() < b->getX() + b->getGrid() &&
            player->getY() + player->getGrid() > b->getY() &&
            player->getY() < b->getY() + b->getGrid())
            {
                // Respawn on hit, will implement 
                // a black screen, music reset and stuff later
                player->setRhyBar(player->getRhyBar() - 30);
            }

            // Collision with attack hurtbox, works even when invul
            // If currently attacking, and hitting breakable block
            // Then overlapping hitbox, the usual
            if (player->getRhyAtk() && b->getType() == 4 &&
            player->getRhyAtkX() + player->getRhyAtkGrid() > b->getX() &&
            player->getRhyAtkX() < b->getX() + b->getGrid() &&
            player->getRhyAtkY() + player->getRhyAtkGrid() > b->getY() &&
            player->getRhyAtkY() < b->getY() + b->getGrid())
            {
                
            }
        }
    }
}

// Horizontal shooter stuff
void Collision::horiPlayerEnemyColli(Player *player, Enemy *enemy)
{
    // Loop through all 3 smallboxes
    for (int i = 0; i < 3; i++)
    {
        if (player->getSmallboxX(i) < enemy->getX() + enemy->getGrid() &&
        player->getSmallboxX(i) + player->getSmallboxWidth() > enemy->getX() &&
        player->getSmallboxY(i) < enemy->getY() + enemy->getGrid() &&
        player->getSmallboxY(i) + player->getSmallboxHeight() > enemy->getY())
        {
            player->setHealth(player->getHealth() - 1);
        }
    }
}
void Collision::horiPlayerProjColli(Player *player, Projectile *proj)
{
    // Loop through all 3 smallboxes
    for (int i = 0; i < 3; i++)
    {
        if (player->getSmallboxX(i) < proj->getX() + proj->getGrid() &&
        player->getSmallboxX(i) + player->getSmallboxWidth() > proj->getX() &&
        player->getSmallboxY(i) < proj->getY() + proj->getGrid() &&
        player->getSmallboxY(i) + player->getSmallboxHeight() > proj->getY())
        {
            player->setHealth(player->getHealth() - 1);
        }
    }
}

// Vertical shooter stuff
void Collision::vertPlayerEnemyColli(Player *player, Enemy *enemy)
{
    // Every object is a square now with how I use grid collision lol
    if (player->getX() < enemy->getX() + enemy->getGrid() &&
    player->getX() + player->getX() > enemy->getX() &&
    player->getY() < enemy->getY() + enemy->getGrid() &&
    player->getY() + player->getGrid() > enemy->getY())
    {
        // Do something when player collide with enemy
        player->setHealth(player->getHealth() - 1);
        
    }
}
void Collision::vertPlayerProjColli(Player *player, Projectile *proj)
{
    if (player->getX() < proj->getX() + proj->getGrid() &&
    player->getX() + player->getX() > proj->getX() &&
    player->getY() < proj->getY() + proj->getGrid() &&
    player->getY() + player->getGrid() > proj->getY())
    {
        // Do something when player collide with proj
        player->setHealth(player->getHealth() - 1);
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
{ 
    std::cout << "Collision terminated!\n";
}