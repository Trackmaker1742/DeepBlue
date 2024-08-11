#include "renderer.h"

Renderer::Renderer(Scene *sc, Camera *c) : 
    scene(sc),
    // player(p), 
    cam(c)
    // stage(stg)
{ }

// This is about to get messy
void Renderer::renderPlayer(Player *player, float dt)
{
    // Assign value to render coordinates
    if(player->getRight()) player->setRenderX(player->getX() - player->getGrid() * 0.59375);
    else player->setRenderX(player->getX());
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
            idle_counter += dt * 4;
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
            if (abs(player->getVelX()) < player->getVelXMax() * 0.25) run_counter += dt * 5;
            else if (abs(player->getVelX()) < player->getGrid() * 0.5) run_counter += dt * 8;
            else if (abs(player->getVelX()) < player->getVelXMax() * 0.75) run_counter += dt * 11;
            else if (abs(player->getVelX()) < player->getVelXMax()) run_counter += dt * 14;
            else if (abs(player->getVelX()) >= player->getVelXMax()) run_counter += dt * 17;
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
        looped_counter += dt * 6;
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
        looped_counter += dt * 6;
        if (looped_counter > 2) looped_counter = 0;
        src_rect = {
            (player->getRight() ? 0 : 13) * 64 + int(looped_counter) * 64, 
            320,
            64,
            64
        };
    }

    // Actions with no looping sprites 
    // (jump start, peak jump height, landing)
    if (player->getJumpStart())
    {
        temp_counter += dt * 30;
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

        temp_counter += dt * 7;
        if (temp_counter > 1)
        {
            temp_counter = 0;
            player->setApex(false);
        }
        src_rect = {
            (player->getRight() ? 0 : 13) * 64 + int(looped_counter) * 64, 
            256,
            64,
            64
        };
    }
    if (player->getLand())
    {
        // Reset shared counter between ascend and descend
        looped_counter = 0;

        temp_counter += dt * 12;
        if (temp_counter > 2)
        {
            temp_counter = 0;
            player->setLand(false);
        }
        src_rect = {
            (player->getRight() ? 0 : 13) * 64 + int(temp_counter) * 64, 
            384,
            64,
            64
        };
    }

    des_rect = {
        cam->getRendX() + r_cam_offset_x, 
        scene->getHeight() - player->getGrid() - cam->getRendY() - r_cam_offset_y, 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderCopy(scene->getRenderer(), player->getTexture(), &src_rect, &des_rect);

    // Render the hitbox
    SDL_SetRenderDrawBlendMode(scene->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(scene->getRenderer(), 255, 0, 0, 150);
    des_rect = {
        cam->getRendX() + p_cam_offset_x, 
        scene->getHeight() - player->getHeight() - cam->getRendY() - p_cam_offset_y, 
        player->getWidth(),
        player->getHeight()
    };
    SDL_RenderFillRect(scene->getRenderer(), &des_rect);
}

void Renderer::renderBackground(Stage *stage, Player *player)
{
    // Set anchor for moving bg
    if (!init_x) 
    {
        initial_x = stage->getRespX();
        init_x = true;
    }
    // Render background
    // 2 background image will be rendered at the same time
    int des1 = - int((player->getX() - initial_x)); // Starts at 0
    int des2 = scene->getWidth() + des1;            // Starts at end of screen
    // Once one bg is no longer in view, 
    // it will loop back or forward to keep covering the screen
    while (des1 < -scene->getWidth()) des1 += 2 * scene->getWidth();
    while (des2 < -scene->getWidth()) des2 += 2 * scene->getWidth();
    // Render backgrounds 
    // (the plan is to have around 4-5 layers per stage, then 2 times each bg element in the render)
    des_rect = {des1, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), stage->getBackground(), NULL, &des_rect);
    des_rect = {des2, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), stage->getBackground(), NULL, &des_rect);
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