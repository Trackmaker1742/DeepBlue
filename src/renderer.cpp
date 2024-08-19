#include "renderer.h"

Renderer::Renderer(Scene *sc, Camera *c) : 
    scene(sc),
    // player(p), 
    cam(c)
    // stage(stg)
{ }

// This is about to get messy
void Renderer::renderPlatformer(Player *player)
{
    // Assign value to render coordinates
    if(player->getRight()) player->setRenderX(player->getX() - player->getGrid() * 0.40625);
    else player->setRenderX(player->getX() - player->getGrid() * 0.1875);
    player->setRenderY(player->getY());

    // Values for offsetting the camera from the player when in the edges of the stage
    int16_t r_cam_offset_x = player->getRenderX() - cam->getX();
    int16_t r_cam_offset_y = player->getRenderY() - cam->getY();
    
    int16_t p_cam_offset_x = player->getX() - cam->getX();
    int16_t p_cam_offset_y = player->getY() - cam->getY();

    // Idling
    if (player->getVelX() == 0)
    {
        run_counter = 0;

        if (player->getVelY() == 0 && !player->getLand())
        {
            idle_counter += scene->getDeltaTime() * 4;
            if (idle_counter > 11) idle_counter = 0;
            src_rect = {
                (player->getRight() ? 0 : 13) * 64 + int(idle_counter) * 64, 
                0,
                64,
                64
            };
        }
    }
    // Moving
    else 
    {
        idle_counter = 0;

        if (player->getVelY() == 0 && !player->getLand())
        {
            // Higher the speed, the faster the sprite switches
            if (abs(player->getVelX()) < player->getVelXMax() * 0.25) run_counter += scene->getDeltaTime() * 5;
            else if (abs(player->getVelX()) < player->getGrid() * 0.5) run_counter += scene->getDeltaTime() * 8;
            else if (abs(player->getVelX()) < player->getVelXMax() * 0.75) run_counter += scene->getDeltaTime() * 11;
            else if (abs(player->getVelX()) < player->getVelXMax()) run_counter += scene->getDeltaTime() * 14;
            else if (abs(player->getVelX()) >= player->getVelXMax()) run_counter += scene->getDeltaTime() * 17;
            if (run_counter > 7) run_counter = 0;

            // Dynamically switching between parts of sprite sheet
            // Probably just for run mode
            src_rect = {
                (player->getRight() ? 0 : 13) * 64 + int(run_counter) * 64, 
                64,
                64,
                64
            };
        }
    }

    // Ascend, descend
    // (looped sprites, use looped counter)
    if (player->getAscend())
    {
        looped_counter += scene->getDeltaTime() * 6;
        if (looped_counter > 2) looped_counter = 0;
        src_rect = {
            (player->getRight() ? 0 : 13) * 64 + int(looped_counter) * 64, 
            192,
            64,
            64
        };
    }
    if (player->getDescend())
    {
        looped_counter += scene->getDeltaTime() * 6;
        if (looped_counter > 2) looped_counter = 0;
        src_rect = {
            5 * 64 + (player->getRight() ? 0 : 13) * 64 + int(looped_counter) * 64, 
            128,
            64,
            64
        };
    }    
    // Wall (can work with looped counter)
    if (player->getOnWall())
    {
        if (player->getClimbUp()) looped_counter += scene->getDeltaTime() * 4;
        else if (player->getClimbDown()) looped_counter -= scene->getDeltaTime() * 4;
        
        if (looped_counter > 3) looped_counter = 0;
        if (looped_counter < 0) looped_counter = 3;
        
        src_rect = {
            8 * 64 + (player->getRight() ? 0 : 13) * 64 + int(looped_counter) * 64, 
            64,
            64,
            64
        };
    }

    // Actions with no looping sprites 
    // (jump start, peak jump height, landing)
    if (player->getJumpStart())
    {
        if (!jump_anchor && player->getOnGround())
        {
            player_jump_x = player->getX();
            player_jump_y = player->getY();
            jump_anchor = true;
        }
        temp_counter += scene->getDeltaTime() * 25;
        if (temp_counter > 2)
        {
            temp_counter = 0;
            player->setJumpStart(false);
        }
        src_rect = {
            (player->getRight() ? 0 : 13) * 64 + int(temp_counter) * 64, 
            128,
            64,
            64
        };
    }
    if (player->getApex())
    {
        // Reset shared counter between ascend and descend
        looped_counter = 0;

        temp_counter += scene->getDeltaTime() * 7;
        if (temp_counter > 1)
        {
            temp_counter = 0;
            player->setApex(false);
        }
        src_rect = {
            3 * 64 + (player->getRight() ? 0 : 13) * 64 + int(looped_counter) * 64, 
            128,
            64,
            64
        };
    }
    if (player->getLand())
    {
        // Reset shared counter between ascend and descend
        looped_counter = 0;

        temp_counter += scene->getDeltaTime() * 12;
        if (temp_counter > 2)
        {
            temp_counter = 0;
            player->setLand(false);
        }
        src_rect = {
            5 * 64 + (player->getRight() ? 0 : 13) * 64 + int(temp_counter) * 64, 
            192,
            64,
            64
        };
    }
    if (player->getOnDash())
    {
        if (!dash_anchor)
        {
            player_dash_x = player->getX();
            player_dash_y = player->getY();
            player_temp_right = player->getRight();
            player_temp_on_ground = player->getOnGround();
            dash_anchor = true;
        }
        src_rect = {
            8 * 64 + (player->getRight() ? 0 : 13) * 64, 
            128,
            64,
            64
        };
    }
    if (player->getDashHalt())
    {
        temp_counter += scene->getDeltaTime() * 10;

        if (temp_counter > 1)
        {
            temp_counter = 0;
            player->setDashHalt(false);
        }
        src_rect = {
            8 * 64 + (player->getRight() ? 0 : 13) * 64 + int(temp_counter) * 64, 
            192,
            64,
            64
        };
    } 
    // Dash effect processing
    if (dash_anchor)
    {
        dash_effect_counter += scene->getDeltaTime() * 10;
        if (dash_effect_counter > 3)
        {
            dash_effect_counter = 0;
            dash_anchor = false;
        }
        src_rect_d = {
            (player_temp_on_ground ? 0 : 4) * 64 + (player_temp_right ? 0 : 13) * 64 + int(dash_effect_counter) * 64,
            256,
            64,
            64
        };
    }
    // Jump effect processing
    if (jump_anchor)
    {
        jump_effect_counter += scene->getDeltaTime() * 10;
        if (jump_effect_counter > 4)
        {
            jump_effect_counter = 0;
            jump_anchor = false;
        }
        src_rect_j = {
            12 * 64,
            int(jump_effect_counter) * 64,
            64,
            64
        };
    }

    // Render sprites
    des_rect = {
        cam->getRendX() + r_cam_offset_x, 
        scene->getHeight() - player->getGrid() - cam->getRendY() - r_cam_offset_y, 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderCopy(scene->getRenderer(), player->getTexture(), &src_rect, &des_rect);
    // Render effects
    // Dash effect
    if (dash_anchor)
    {
        des_rect_d = {
            cam->getRendX() + int(player_dash_x - (player_temp_right ? player->getGrid() * 0.75 : (-player->getWidth() + player->getGrid() * 0.25)) - cam->getX()), 
            scene->getHeight() - player->getGrid() - cam->getRendY() - int(player_dash_y - cam->getY()) + int(player_temp_on_ground ? 0 : player->getGrid() * 0.2), 
            player->getGrid(), 
            player->getGrid()
        };
        SDL_RenderCopy(scene->getRenderer(), player->getTexture(), &src_rect_d, &des_rect_d);
    }
    // Jump effect
    if (jump_anchor)
    {
        des_rect_j = {
            cam->getRendX() + int(player_jump_x - player->getWidth() * 3/4 - cam->getX()), 
            scene->getHeight() - player->getGrid() - cam->getRendY() - int(player_jump_y - cam->getY()), 
            player->getGrid(), 
            player->getGrid()
        };
        SDL_RenderCopy(scene->getRenderer(), player->getTexture(), &src_rect_j, &des_rect_j);
    }

    // // Render the hitbox
    // SDL_SetRenderDrawBlendMode(scene->getRenderer(), SDL_BLENDMODE_BLEND);
    // SDL_SetRenderDrawColor(scene->getRenderer(), 255, 0, 0, 150);
    // des_rect = {
    //     cam->getRendX() + p_cam_offset_x, 
    //     scene->getHeight() - player->getHeight() - cam->getRendY() - p_cam_offset_y, 
    //     player->getWidth(),
    //     player->getHeight()
    // };
    // SDL_RenderFillRect(scene->getRenderer(), &des_rect);
}

void Renderer::renderVertShooter(Player *player)
{
    idle_counter += scene->getDeltaTime() * 4;
    if (idle_counter > 11) idle_counter = 0;
    src_rect = {
        int(idle_counter) * 64, 
        (player->getRight() ? 0 : 1) * 64,
        64,
        64
    };

    des_rect = {
        int(player->getX()), 
        scene->getHeight() - player->getGrid() - int(player->getY()), 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderCopy(scene->getRenderer(), player->getTexture(), &src_rect, &des_rect);

    // Render the hitbox
    SDL_SetRenderDrawBlendMode(scene->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(scene->getRenderer(), 255, 0, 0, 150);
    des_rect = {
        int(player->getX()), 
        scene->getHeight() - player->getGrid() - int(player->getY()), 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderFillRect(scene->getRenderer(), &des_rect);
}

void Renderer::renderHoriShooter(Player *player)
{
    idle_counter += scene->getDeltaTime() * 5;
    if (idle_counter > 5) idle_counter = 0;
    src_rect = {
        int(idle_counter) * 96, 
        0,
        96,
        96
    };

    des_rect = {
        int(player->getX()), 
        scene->getHeight() - player->getGrid() - int(player->getY()), 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderCopy(scene->getRenderer(), player->getTexture(), &src_rect, &des_rect);

    // // Render the hitbox
    // SDL_SetRenderDrawBlendMode(scene->getRenderer(), SDL_BLENDMODE_BLEND);
    // SDL_SetRenderDrawColor(scene->getRenderer(), 255, 0, 0, 150);
    // des_rect = {
    //     int(player->getX()), 
    //     scene->getHeight() - player->getGrid() - int(player->getY()), 
    //     player->getGrid(), 
    //     player->getGrid()
    // };
    // SDL_RenderFillRect(scene->getRenderer(), &des_rect);
}

void Renderer::renderRhythm(Player *player)
{
    idle_counter += scene->getDeltaTime() * 4;
    if (idle_counter > 2) idle_counter = 0;
    src_rect = {
        int(idle_counter) * 64, 
        (player->getRight() ? 0 : 1) * 64,
        64,
        64
    };

    des_rect = {
        int(player->getX()), 
        scene->getHeight() - player->getGrid() - int(player->getY()), 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderCopy(scene->getRenderer(), player->getTexture(), &src_rect, &des_rect);

    // Render the hitbox
    SDL_SetRenderDrawBlendMode(scene->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(scene->getRenderer(), 255, 0, 0, 150);
    des_rect = {
        int(player->getX()), 
        scene->getHeight() - player->getGrid() - int(player->getY()), 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderFillRect(scene->getRenderer(), &des_rect);
}

void Renderer::renderBackground(Stage *stage, Player *player)
{
    if (!init_x) 
    {
        initial_x = stage->getRespX();
        init_x = true;
    }
    // Render background
    // 2 background image will be rendered at the same time
    // Divide des1 to slow down the bg speed
    int des1 = - int((player->getX() - initial_x)) / 8;    // Starts at 0
    int des2 = scene->getWidth() + des1;                    // Starts at end of screen
    // Once one bg is no longer in view, 
    // it will loop back or forward to keep covering the screen
    while (des1 < -scene->getWidth()) des1 += 2 * scene->getWidth();
    while (des2 < -scene->getWidth()) des2 += 2 * scene->getWidth();
    // Render backgrounds 
    // (the plan is to have around 4-5 layers per stage, then 2 times each bg element in the render)
    des_rect = {des1, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), stage->getBackgroundLayers()[0], NULL, &des_rect);
    des_rect = {des2, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), stage->getBackgroundLayers()[0], NULL, &des_rect);

    // Layer 2
    if (layer2_i < scene->getWidth() && stage->getBgMove()) layer2_i += player->getGrid() * scene->getDeltaTime() / 18;
    else layer2_i = 0;
    des1 = - int((player->getX() - initial_x)) / 7 - layer2_i;
    des2 = scene->getWidth() + des1;
    while (des1 < -scene->getWidth()) des1 += 2 * scene->getWidth();
    while (des2 < -scene->getWidth()) des2 += 2 * scene->getWidth();
    des_rect = {des1, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), stage->getBackgroundLayers()[1], NULL, &des_rect);
    des_rect = {des2, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), stage->getBackgroundLayers()[1], NULL, &des_rect);

    // Layer 3
    if (layer3_i < scene->getWidth() && stage->getBgMove()) layer3_i += player->getGrid() * scene->getDeltaTime() / 14;
    else layer3_i = 0;
    des1 = - int((player->getX() - initial_x)) / 6 - layer3_i;
    des2 = scene->getWidth() + des1;
    while (des1 < -scene->getWidth()) des1 += 2 * scene->getWidth();
    while (des2 < -scene->getWidth()) des2 += 2 * scene->getWidth();
    des_rect = {des1, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), stage->getBackgroundLayers()[2], NULL, &des_rect);
    des_rect = {des2, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), stage->getBackgroundLayers()[2], NULL, &des_rect);
    
    // Layer 4
    if (layer4_i < scene->getWidth() && stage->getBgMove()) layer4_i += player->getGrid() * scene->getDeltaTime() / 10;
    else layer4_i = 0;
    des1 = - int((player->getX() - initial_x)) / 5 - layer4_i;
    des2 = scene->getWidth() + des1;
    while (des1 < -scene->getWidth()) des1 += 2 * scene->getWidth();
    while (des2 < -scene->getWidth()) des2 += 2 * scene->getWidth();
    des_rect = {des1, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), stage->getBackgroundLayers()[3], NULL, &des_rect);
    des_rect = {des2, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), stage->getBackgroundLayers()[3], NULL, &des_rect);
}

void Renderer::renderStage(Stage *stage, Player *player)
{
    // Render background
    renderBackground(stage, player);

    // Camera stuff for other renders
    int16_t delta_x = cam->getRendX() - cam->getX();
    int16_t delta_y = cam->getRendY() - cam->getY();

    // Render blocks
    for (Block *b : stage->getBlockVec())
    {
        // Skip blocks that are outside of the screen
        if (int(b->getGridX()) + delta_x < -b->getGrid() || 
        scene->getHeight() - b->getGrid() - (int(b->getGridY()) + delta_y) > scene->getHeight() ||
        int(b->getGridX()) + delta_x > scene->getWidth() + b->getGrid() ||
        scene->getHeight() - b->getGrid() - (int(b->getGridY()) + delta_y) < -b->getGrid())
            continue;

        des_rect = {
            int(b->getGridX()) + delta_x, 
            scene->getHeight() - b->getGrid() - (int(b->getGridY()) + delta_y), 
            b->getGrid(), 
            b->getGrid()
        };
        SDL_RenderCopy(scene->getRenderer(), b->getTexture(), NULL, &des_rect);
    }
    // Render projectiles
    for (Projectile *p : stage->getProjVec())
    {
        // Skip projectiles that are outside of the screen
        if (int(p->getGridX()) + delta_x < -p->getGrid() || 
        scene->getHeight() - p->getGrid() - (int(p->getGridY()) + delta_y) > scene->getHeight() ||
        int(p->getGridX()) + delta_x > scene->getWidth() + p->getGrid() ||
        scene->getHeight() - p->getGrid() - (int(p->getGridY()) + delta_y) < -p->getGrid())
            continue;

        des_rect = {
            int(p->getGridX()) + delta_x, 
            scene->getHeight() - p->getGrid() - (int(p->getGridY()) + delta_y), 
            p->getGrid(), 
            p->getGrid()
        };
        SDL_RenderCopy(scene->getRenderer(), p->getTexture(), NULL, &des_rect);
    }
}

// Menu render has to be run here to keep consistency
// And moving bg is pretty long anyway
// Separating input from render is a good practice as well
void Renderer::renderMainMenu()
{
    // Render background
    des_rect = {0, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), scene->getBackground(0), NULL, &des_rect);
}

void Renderer::renderStageSelect()
{
    // Render background
    des_rect = {0, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), scene->getBackground(1), NULL, &des_rect);
}

void Renderer::renderGallery()
{
    // Render background
    des_rect = {0, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), scene->getBackground(2), NULL, &des_rect);
}

void Renderer::renderSettings()
{
    // Render background
    des_rect = {0, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), scene->getBackground(3), NULL, &des_rect);
}

void Renderer::renderPauseMenu()
{
    // Render background
    des_rect = {0, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), scene->getBackground(4), NULL, &des_rect);
}