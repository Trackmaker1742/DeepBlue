#include "renderer.h"

Renderer::Renderer(Config *sc, Camera *c) : 
    config(sc),
    // player(p), 
    cam(c)
    // stage(stg)
{ }

// This is about to get messy
void Renderer::renderPlayerPlat(Player *player)
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
            idle_counter += config->getDeltaTime() * 4;
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
            if (abs(player->getVelX()) < player->getVelXMax() * 0.25) run_counter += config->getDeltaTime() * 5;
            else if (abs(player->getVelX()) < player->getGrid() * 0.5) run_counter += config->getDeltaTime() * 8;
            else if (abs(player->getVelX()) < player->getVelXMax() * 0.75) run_counter += config->getDeltaTime() * 11;
            else if (abs(player->getVelX()) < player->getVelXMax()) run_counter += config->getDeltaTime() * 14;
            else if (abs(player->getVelX()) >= player->getVelXMax()) run_counter += config->getDeltaTime() * 17;
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
        looped_counter += config->getDeltaTime() * 6;
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
        looped_counter += config->getDeltaTime() * 6;
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
        if (player->getClimbUp()) looped_counter += config->getDeltaTime() * 4;
        else if (player->getClimbDown()) looped_counter -= config->getDeltaTime() * 4;
        
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
        temp_counter += config->getDeltaTime() * 25;
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

        temp_counter += config->getDeltaTime() * 7;
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

        temp_counter += config->getDeltaTime() * 12;
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
        temp_counter += config->getDeltaTime() * 10;

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
        dash_effect_counter += config->getDeltaTime() * 10;
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
        jump_effect_counter += config->getDeltaTime() * 10;
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
        config->getHeight() - player->getGrid() - cam->getRendY() - r_cam_offset_y, 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderCopy(config->getRenderer(), player->getTexture(), &src_rect, &des_rect);
    // Render effects
    // Dash effect
    if (dash_anchor)
    {
        des_rect_d = {
            cam->getRendX() + int(player_dash_x - (player_temp_right ? player->getGrid() * 0.75 : (-player->getWidth() + player->getGrid() * 0.25)) - cam->getX()), 
            config->getHeight() - player->getGrid() - cam->getRendY() - int(player_dash_y - cam->getY()) + int(player_temp_on_ground ? 0 : player->getGrid() * 0.2), 
            player->getGrid(), 
            player->getGrid()
        };
        SDL_RenderCopy(config->getRenderer(), player->getTexture(), &src_rect_d, &des_rect_d);
    }
    // Jump effect
    if (jump_anchor)
    {
        des_rect_j = {
            cam->getRendX() + int(player_jump_x - player->getWidth() * 3/4 - cam->getX()), 
            config->getHeight() - player->getGrid() - cam->getRendY() - int(player_jump_y - cam->getY()), 
            player->getGrid(), 
            player->getGrid()
        };
        SDL_RenderCopy(config->getRenderer(), player->getTexture(), &src_rect_j, &des_rect_j);
    }

    // // Render the hitbox
    // SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);
    // SDL_SetRenderDrawColor(config->getRenderer(), 255, 0, 0, 150);
    // des_rect = {
    //     cam->getRendX() + p_cam_offset_x, 
    //     config->getHeight() - player->getHeight() - cam->getRendY() - p_cam_offset_y, 
    //     player->getWidth(),
    //     player->getHeight()
    // };
    // SDL_RenderFillRect(config->getRenderer(), &des_rect);
}

void Renderer::renderPlayerVertShooter(Player *player)
{
    idle_counter += config->getDeltaTime() * 4;
    if (idle_counter > 11) idle_counter = 0;
    src_rect = {
        int(idle_counter) * 64, 
        (player->getRight() ? 0 : 1) * 64,
        64,
        64
    };

    des_rect = {
        int(player->getX()), 
        config->getHeight() - player->getGrid() - int(player->getY()), 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderCopy(config->getRenderer(), player->getTexture(), &src_rect, &des_rect);

    // // Render the hitbox
    // SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);
    // SDL_SetRenderDrawColor(config->getRenderer(), 255, 0, 0, 150);
    // des_rect = {
    //     int(player->getX()), 
    //     config->getHeight() - player->getGrid() - int(player->getY()), 
    //     player->getGrid(), 
    //     player->getGrid()
    // };
    // SDL_RenderFillRect(config->getRenderer(), &des_rect);
}

void Renderer::renderPlayerHoriShooter(Player *player)
{
    idle_counter += config->getDeltaTime() * 5;
    if (idle_counter > 5) idle_counter = 0;
    src_rect = {
        int(idle_counter) * 96,
        0,
        96,
        96
    };

    des_rect = {
        int(player->getX() - player->getGrid() / 6), 
        config->getHeight() - player->getGrid() - int(player->getY() - player->getGrid() / 6), 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderCopy(config->getRenderer(), player->getTexture(), &src_rect, &des_rect);

    // // Render big hitbox
    // SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);
    // SDL_SetRenderDrawColor(config->getRenderer(), 255, 255, 255, 150);
    // des_rect = {
    //     int(player->getX()), 
    //     config->getHeight() - int(player->getHeight()) - int(player->getY()), 
    //     player->getWidth(), 
    //     player->getHeight()
    // };
    // SDL_RenderFillRect(config->getRenderer(), &des_rect);
    
    // // Render small hitbox 1
    // SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);
    // SDL_SetRenderDrawColor(config->getRenderer(), 255, 0, 0, 150);
    // des_rect = {
    //     int(player->getSmallboxX(0)), 
    //     config->getHeight() - int(player->getSmallboxHeight()) - int(player->getSmallboxY(0)),
    //     int(player->getSmallboxWidth()), 
    //     int(player->getSmallboxHeight())
    // };
    // SDL_RenderFillRect(config->getRenderer(), &des_rect);
    
    // // Render small hitbox 2
    // SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);
    // SDL_SetRenderDrawColor(config->getRenderer(), 255, 255, 0, 150);
    // des_rect = {
    //     int(player->getSmallboxX(1)), 
    //     config->getHeight() - int(player->getSmallboxHeight()) - int(player->getSmallboxY(1)),
    //     int(player->getSmallboxWidth()), 
    //     int(player->getSmallboxHeight())
    // };
    // SDL_RenderFillRect(config->getRenderer(), &des_rect);
    
    // // Render small hitbox 3
    // SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);
    // SDL_SetRenderDrawColor(config->getRenderer(), 0, 0, 255, 150);
    // des_rect = {
    //     int(player->getSmallboxX(2)), 
    //     config->getHeight() - int(player->getSmallboxHeight()) - int(player->getSmallboxY(2)),
    //     int(player->getSmallboxWidth()), 
    //     int(player->getSmallboxHeight())
    // };
    // SDL_RenderFillRect(config->getRenderer(), &des_rect);
}

void Renderer::renderPlayerRhythm(Player *player)
{
    idle_counter += config->getDeltaTime() * 4;
    if (idle_counter > 2) idle_counter = 0;
    src_rect = {
        int(idle_counter) * 64, 
        (player->getRight() ? 0 : 1) * 64,
        64,
        64
    };

    des_rect = {
        int(player->getX()), 
        config->getHeight() - player->getGrid() - int(player->getY()), 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderCopy(config->getRenderer(), player->getTexture(), &src_rect, &des_rect);

    // Render the hitbox
    SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(config->getRenderer(), 255, 0, 0, 150);
    des_rect = {
        int(player->getX()), 
        config->getHeight() - player->getGrid() - int(player->getY()), 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderFillRect(config->getRenderer(), &des_rect);
}

void Renderer::renderBackground(Stage *stage, Player *player)
{
    if (!init_x) 
    {
        initial_x = 0;
        init_x = true;
    }
    // Init layers' iterators for auto background scrolling
    if (!layer_auto_i_init)
    {
        for (int i = 0; i < stage->getBgCountMax(); i++)
        {
            layer_auto_i.push_back(0);
        }
        layer_auto_i_init = true;
    }
    // Render background
    // 2 background image will be rendered at the same time
    // Divide des1 to slow down the bg speed
    float des1, des2;
    for (int i = 0; i < stage->getBgCountMax(); i++)
    {
        // Auto-scroll movement
        if (stage->getBgParam(i, 0))
        {
            if (layer_auto_i[i] < config->getWidth())
            {
                layer_auto_i[i] += player->getGrid() * stage->getBgParam(i, 0) * config->getDeltaTime() / 500;
            }
            else layer_auto_i[i] = 0;
        }
        // Background movement based on player movement
        des1 = - (player->getX() - initial_x) * stage->getBgParam(i, 1) / 100  - layer_auto_i[i];   // Starts at 0
        des2 = config->getWidth() + des1;    // Starts at end of screen
        // Once one bg is no longer in view, 
        // it will loop back or forward to keep covering the screen
        while (des1 < -config->getWidth()) des1 += 2 * config->getWidth();
        while (des2 < -config->getWidth()) des2 += 2 * config->getWidth();
        // Render backgrounds 
        // (the plan is to have around 4-5 layers per stage, then 2 times each bg element in the render)
        des_rect_f = {des1, 0, float(config->getWidth()), float(config->getHeight())};
        SDL_RenderCopyF(config->getRenderer(), stage->getBackgroundLayers()[i], NULL, &des_rect_f);
        des_rect_f = {des2, 0, float(config->getWidth()), float(config->getHeight())};
        SDL_RenderCopyF(config->getRenderer(), stage->getBackgroundLayers()[i], NULL, &des_rect_f);
    }
}

void Renderer::renderBlocks(Stage *stage, Player *player)
{
    for (Block *b : stage->getBlockVec())
    {
        // Skip blocks that are outside of the screen
        // Set on screen state for less intensive collision checking
        if (int(b->getX()) + delta_x < -b->getGrid() || 
        config->getHeight() - b->getGrid() - (int(b->getY()) + delta_y) > config->getHeight() ||
        int(b->getX()) + delta_x > config->getWidth() + b->getGrid() ||
        config->getHeight() - b->getGrid() - (int(b->getY()) + delta_y) < -b->getGrid())
        {
            b->setOnScreen(false);
            continue;
        }
        else b->setOnScreen(true);
    
        des_rect = {
            int(b->getX()) + delta_x, 
            config->getHeight() - b->getGrid() - (int(b->getY()) + delta_y), 
            b->getGrid(), 
            b->getGrid()
        };
        SDL_RenderCopy(config->getRenderer(), b->getTexture(), NULL, &des_rect);
    }
}
void Renderer::renderMovingBlocks(Stage *stage, Player *player)
{
    for (Block *b : stage->getMovingBlockVec())
    {
        // Skip blocks that are outside of the screen
        if (int(b->getX()) + delta_x < -b->getGrid() || 
        config->getHeight() - b->getGrid() - (int(b->getY()) + delta_y) > config->getHeight() ||
        int(b->getX()) + delta_x > config->getWidth() + b->getGrid() ||
        config->getHeight() - b->getGrid() - (int(b->getY()) + delta_y) < -b->getGrid())
            continue;

        des_rect = {
            int(b->getX()) + delta_x, 
            config->getHeight() - b->getGrid() - (int(b->getY()) + delta_y), 
            b->getGrid(), 
            b->getGrid()
        };
        SDL_RenderCopy(config->getRenderer(), b->getTexture(), NULL, &des_rect);
    }
}
void Renderer::renderProjectiles(Stage *stage, Player *player)
{
    for (auto p : player->getProjectiles())
    {
        // Skip projectiles that are outside of the screen
        if (int(p->getX()) + delta_x < -p->getGrid() || 
        config->getHeight() - p->getGrid() - (int(p->getY()) + delta_y) > config->getHeight() ||
        int(p->getX()) + delta_x > config->getWidth() + p->getGrid() ||
        config->getHeight() - p->getGrid() - (int(p->getY()) + delta_y) < -p->getGrid())
            continue;

        des_rect = {
            int(p->getX()) + delta_x, 
            config->getHeight() - p->getGrid() - (int(p->getY()) + delta_y), 
            p->getGrid(), 
            p->getGrid()
        };
        SDL_RenderCopy(config->getRenderer(), p->getTexture(), NULL, &des_rect);
    }
}

void Renderer::renderStagePlat(Stage *stage, Player *player, Editor *edit)
{
    // Render background
    renderBackground(stage, player);

    // Camera stuff for other renders
    delta_x = cam->getRendX() - cam->getX();
    delta_y = cam->getRendY() - cam->getY();

    // Render blocks
    renderBlocks(stage, player);
    // Render moving blocks
    renderMovingBlocks(stage, player);
    // Render editor mode
    if (player->getEditor()) 
    {
        // Render grid array
        renderGridLines(stage, player, edit);
        renderEditorMenu(stage, edit);
    }
    // Render player
    renderPlayerPlat(player);
}
void Renderer::renderStageShooter(Stage *stage, Player *player)
{
    // Render background
    renderBackground(stage, player);

    // Camera stuff for other renders
    delta_x = cam->getRendX() - cam->getX();
    delta_y = cam->getRendY() - cam->getY();

    // Render blocks
    renderBlocks(stage, player);
    // Render moving blocks
    renderMovingBlocks(stage, player);
    // Render projectiles
    renderProjectiles(stage, player);
}
void Renderer::renderStageRhythm(Stage *stage, Player *player)
{
    // Render background
    renderBackground(stage, player);

    // Camera stuff for other renders
    delta_x = cam->getRendX() - cam->getX();
    delta_y = cam->getRendY() - cam->getY();

    // Render blocks
    renderBlocks(stage, player);
    // Render moving blocks
    renderMovingBlocks(stage, player);
    // Render player
    renderPlayerRhythm(player);
}

void Renderer::renderGridLines(Stage *stage, Player *player, Editor *edit)
{
    // Render grid lines for level creation purposes
    SDL_SetRenderDrawColor(config->getRenderer(), 255, 0, 0, 255);

    // With the player at the center, the grid array will start from that minus the screen width
    // and end at the player x + screen width
    int16_t array_start = (player->getX() - config->getWidth()) / (player->getGrid() / 2);
    int16_t array_end = (player->getX() + config->getWidth()) / (player->getGrid() / 2);
    for (int i = array_start; i < array_end; i++)
    {
        SDL_RenderDrawLine(config->getRenderer(),
            i * (player->getGrid() / 2) + delta_x,
            0,
            i * (player->getGrid() / 2) + delta_x,
            config->getHeight()
        );
    }
    // The same concept applies to vertical gridlines
    // In platforming mode, player has double the grid size of normal blocks
    // Therefore, it needs to be divided by 2 in order to get the right grid size
    array_start = (player->getY() - config->getHeight()) / (player->getGrid() / 2);
    array_end = (player->getY() + config->getHeight()) / (player->getGrid() / 2);
    for (int i = array_start; i < array_end; i++)
    {
        SDL_RenderDrawLine(config->getRenderer(),
            0,
            config->getHeight() - (i * (player->getGrid() / 2) + delta_y),
            config->getWidth(),
            config->getHeight() - (i * (player->getGrid() / 2) + delta_y)
        );
    }
    // Transparent color layer for better visibility
    SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(config->getRenderer(), 192, 192, 192, 144);
    des_rect = {0, 0, config->getWidth(), config->getHeight()};
    SDL_RenderFillRect(config->getRenderer(), &des_rect);

    // Player position highlight
    // Currently, the player character will act as the cursor for block placing
    uint16_t highlight_x = player->getCenterX() / (player->getGrid() / 2);
    uint16_t highlight_y = player->getCenterY() / (player->getGrid() / 2);
    SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);
    if (!edit->getAddMBlock())
        SDL_SetRenderDrawColor(config->getRenderer(), 255, 0, 0, 144);
    else 
    {
        if (edit->getInitMBlock())
            SDL_SetRenderDrawColor(config->getRenderer(), 0, 255, 0, 144);
        else 
            SDL_SetRenderDrawColor(config->getRenderer(), 0, 0, 255, 144);
    }
    des_rect = {
        highlight_x * (player->getGrid() / 2) + delta_x, 
        config->getHeight() - player->getGrid() / 2 - (highlight_y * (player->getGrid() / 2) + delta_y), 
        player->getGrid() / 2, 
        player->getGrid() / 2
    };
    SDL_RenderFillRect(config->getRenderer(), &des_rect);

    // // Camera outline (for easier debugging)
    // SDL_SetRenderDrawColor(config->getRenderer(), 255, 0, 0, 255);
    // // Horizontal lines
    // SDL_RenderDrawLine(config->getRenderer(), 
    //     0,
    //     int(cam->getRendY()),
    //     config->getWidth(),
    //     int(cam->getRendY())
    // );
    // SDL_RenderDrawLine(config->getRenderer(), 
    //     0,
    //     int(cam->getRendY()) + cam->getGrid(),
    //     config->getWidth(),
    //     int(cam->getRendY()) + cam->getGrid()
    // );
    // // Vertical lines
    // SDL_RenderDrawLine(config->getRenderer(), 
    //     int(cam->getRendX()),
    //     0,
    //     int(cam->getRendX()),
    //     config->getHeight()
    // );
    // SDL_RenderDrawLine(config->getRenderer(), 
    //     int(cam->getRendX()) + cam->getGrid(),
    //     0,
    //     int(cam->getRendX()) + cam->getGrid(),
    //     config->getHeight()
    // );
}
void Renderer::renderEditorMenu(Stage *stage, Editor *edit)
{

    // Menu background (solid color for now)
    uint16_t menu_width = config->getWidth() / 3;
    SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(config->getRenderer(), 220, 220, 220, 220);
    des_rect =
    {
        config->getWidth() - menu_width,
        0,
        menu_width,
        config->getHeight()
    };
    SDL_RenderFillRect(config->getRenderer(), &des_rect);
    
    // Current SEM position for scrolling
    current_row = edit->getSEMCount() / item_per_row;
    current_column = edit->getSEMCount() % item_per_row;

    row_offset = last_row - 11;
    if (row_offset < 0) row_offset = 0;
    // Scroll up
    if (current_row < first_row)
    {
        first_row--;
        last_row--;
    }
    // Scroll down
    if (current_row > last_row) 
    {
        first_row++;
        last_row++;
    }
    current_row -= row_offset;

    // Render block list
    for (int i = row_offset * item_per_row; i < stage->getBlockTextures().size(); i++)
    {
        des_rect = 
        {
            (config->getWidth() - menu_width + edit->getGrid()) + (i % item_per_row * edit->getGrid() * 4 / 3),
            edit->getGrid() + ((i / item_per_row - row_offset) * edit->getGrid() * 4 / 3),
            edit->getGrid(),
            edit->getGrid()
        };
        SDL_RenderCopy(config->getRenderer(), stage->getBlockTextures()[i], NULL, &des_rect);
    }

    // Menu buttons, menu blocks, highlight for selected block, etc.
    // Selected block highlight
    // std::cout << stage->getSEMCount() << "\n";
    SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(config->getRenderer(), 255, 0, 0, 144);
    des_rect = 
    {
        (config->getWidth() - menu_width + edit->getGrid()) + (current_column * edit->getGrid() * 4 / 3),
        edit->getGrid() + (current_row * edit->getGrid() * 4 / 3),
        edit->getGrid(),
        edit->getGrid()
    };
    SDL_RenderFillRect(config->getRenderer(), &des_rect);
}

// Menu render has to be run here to keep consistency
// And moving bg is pretty long anyway
// Separating input from render is a good practice as well
void Renderer::renderMainMenu(Scene *scene)
{
    // Render background
    // Moving bg and stuff will be handled later
    des_rect = {0, 0, config->getWidth(), config->getHeight()};
    SDL_RenderCopy(config->getRenderer(), scene->getBgTexture(MenuIndex::MAIN, 0), NULL, &des_rect);
    // Render elements
    SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(config->getRenderer(), 0, 120, 200, 144);
    // Red highlight for gallery, cause it will be a while until I can finish it
    if (scene->getCounter() == 0 || scene->getCounter() == 2) 
        SDL_SetRenderDrawColor(config->getRenderer(), 255, 0, 0, 144);
    int box_width = 350;
    int box_height = 100;
    des_rect = 
    {
        config->getWidth() / 2 - box_width / 2,
        config->getHeight() / 2 + scene->getCounter() * box_height - 25,
        box_width,
        box_height
    };
    SDL_RenderFillRect(config->getRenderer(), &des_rect);
}

void Renderer::renderStageSelect(Scene *scene)
{
    // Render background
    // Moving bg and stuff will be handled later
    des_rect = {0, 0, config->getWidth(), config->getHeight()};
    SDL_RenderCopy(config->getRenderer(), scene->getBgTexture(MenuIndex::STAGESELECT, 0), NULL, &des_rect);
    // Render elements
    SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(config->getRenderer(), 0, 120, 200, 144);
    int box_width = 150;
    int box_height = 400;
    des_rect = 
    {
        150 + scene->getCounter() * (box_width * 2),
        config->getHeight() / 2 - box_height / 2,
        box_width,
        box_height
    };
    SDL_RenderFillRect(config->getRenderer(), &des_rect);
}

// void Renderer::renderGallery(Scene *scene)
// {
//     // Render background
//     des_rect = {0, 0, config->getWidth(), config->getHeight()};
//     SDL_RenderCopy(config->getRenderer(), config->getBackground(2), NULL, &des_rect);
// }

SDL_Texture *Renderer::loadTextTexture(const std::string& text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) 
{
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (textSurface == nullptr) 
    {
        std::cout << "Unable to render text surface! TTF_Error: " << TTF_GetError() << "\n";
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (texture == nullptr) 
    {
        std::cout << "Unable to create texture from rendered text! SDL_Error: " << SDL_GetError() << "\n";
    }

    return texture;
}

void Renderer::loadTextureForText()
{
    SDL_Color text_color = {255, 0, 0};
    texture_text = loadTextTexture("funni looking ahh text", font, text_color, config->getRenderer());

    text_rect = {100, 100, 0, 0};
    SDL_QueryTexture(texture_text, nullptr, nullptr, &text_rect.w, &text_rect.h);
}

void Renderer::renderSettings(Scene *scene)
{
    // Render background
    des_rect = {0, 0, config->getWidth(), config->getHeight()};
    SDL_RenderCopy(config->getRenderer(), scene->getBgTexture(MenuIndex::SETTINGS, 0), NULL, &des_rect);

    // Render elements
    SDL_SetRenderDrawColor(config->getRenderer(), 0, 120, 200, 144);
    int box_width = 300;
    int box_height = 125;
    if (scene->getCounter() < 4)
        des_rect = 
        {
            config->getWidth() / 2 + box_width,
            config->getHeight() / 2 - box_height * 5 / 2 + scene->getCounter() * box_height * 5 / 3,
            box_width,
            box_height
        };
    else
        des_rect =
        {
            config->getWidth() / 2 - box_width * 2,
            config->getHeight() / 2 - box_height * 5 / 2 + (scene->getCounter() - 1) * box_height * 5 / 3,
            box_width,
            box_height
        };
    SDL_RenderFillRect(config->getRenderer(), &des_rect);
    
    // Render config data
    SDL_RenderCopy(config->getRenderer(), texture_text, nullptr, &text_rect);
}

void Renderer::renderPauseMenu(Scene *scene)
{
    SDL_SetRenderDrawBlendMode(config->getRenderer(), SDL_BLENDMODE_BLEND);

    // Render background
    des_rect = {0, 0, config->getWidth(), config->getHeight()};
    SDL_RenderCopy(config->getRenderer(), scene->getBgTexture(MenuIndex::PAUSE, 0), NULL, &des_rect);

    // Render elements
    SDL_SetRenderDrawColor(config->getRenderer(), 0, 120, 200, 144);
    int box_width = 400;
    int box_height = 200;
    des_rect = 
    {
        config->getWidth() / 2 - box_width / 2,
        config->getHeight() / 2 - box_height + scene->getCounter() * box_height,
        box_width,
        box_height
    };
    SDL_RenderFillRect(config->getRenderer(), &des_rect);
}

Renderer::~Renderer()
{
    std::cout << "Renderer terminated!\n";
}