#include "renderer.h"

Renderer::Renderer(Scene *sc, Camera *c) : 
    scene(sc),
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

void Renderer::renderPlayerVertShooter(Player *player)
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

void Renderer::renderPlayerHoriShooter(Player *player)
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
        int(player->getX() - player->getGrid() / 6), 
        scene->getHeight() - player->getGrid() - int(player->getY() - player->getGrid() / 6), 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderCopy(scene->getRenderer(), player->getTexture(), &src_rect, &des_rect);

    // Render big hitbox
    SDL_SetRenderDrawBlendMode(scene->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(scene->getRenderer(), 255, 255, 255, 150);
    des_rect = {
        int(player->getX()), 
        scene->getHeight() - int(player->getHeight()) - int(player->getY()), 
        player->getWidth(), 
        player->getHeight()
    };
    SDL_RenderFillRect(scene->getRenderer(), &des_rect);
    
    // Render small hitbox 1
    SDL_SetRenderDrawBlendMode(scene->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(scene->getRenderer(), 255, 0, 0, 150);
    des_rect = {
        int(player->getSmallboxX(0)), 
        scene->getHeight() - int(player->getSmallboxHeight()) - int(player->getSmallboxY(0)),
        int(player->getSmallboxWidth()), 
        int(player->getSmallboxHeight())
    };
    SDL_RenderFillRect(scene->getRenderer(), &des_rect);
    
    // Render small hitbox 2
    SDL_SetRenderDrawBlendMode(scene->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(scene->getRenderer(), 255, 255, 0, 150);
    des_rect = {
        int(player->getSmallboxX(1)), 
        scene->getHeight() - int(player->getSmallboxHeight()) - int(player->getSmallboxY(1)),
        int(player->getSmallboxWidth()), 
        int(player->getSmallboxHeight())
    };
    SDL_RenderFillRect(scene->getRenderer(), &des_rect);
    
    // Render small hitbox 3
    SDL_SetRenderDrawBlendMode(scene->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(scene->getRenderer(), 0, 0, 255, 150);
    des_rect = {
        int(player->getSmallboxX(2)), 
        scene->getHeight() - int(player->getSmallboxHeight()) - int(player->getSmallboxY(2)),
        int(player->getSmallboxWidth()), 
        int(player->getSmallboxHeight())
    };
    SDL_RenderFillRect(scene->getRenderer(), &des_rect);
}

void Renderer::renderPlayerRhythm(Player *player)
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
            if (layer_auto_i[i] < scene->getWidth())
            {
                layer_auto_i[i] += player->getGrid() * stage->getBgParam(i, 0) * scene->getDeltaTime() / 500;
            }
            else layer_auto_i[i] = 0;
        }
        // Background movement based on player movement
        des1 = - (player->getX() - initial_x) * stage->getBgParam(i, 1) / 100  - layer_auto_i[i];   // Starts at 0
        des2 = scene->getWidth() + des1;    // Starts at end of screen
        // Once one bg is no longer in view, 
        // it will loop back or forward to keep covering the screen
        while (des1 < -scene->getWidth()) des1 += 2 * scene->getWidth();
        while (des2 < -scene->getWidth()) des2 += 2 * scene->getWidth();
        // Render backgrounds 
        // (the plan is to have around 4-5 layers per stage, then 2 times each bg element in the render)
        des_rect_f = {des1, 0, float(scene->getWidth()), float(scene->getHeight())};
        SDL_RenderCopyF(scene->getRenderer(), stage->getBackgroundLayers()[i], NULL, &des_rect_f);
        des_rect_f = {des2, 0, float(scene->getWidth()), float(scene->getHeight())};
        SDL_RenderCopyF(scene->getRenderer(), stage->getBackgroundLayers()[i], NULL, &des_rect_f);
    }
}

void Renderer::renderBlocks(Stage *stage, Player *player)
{
    for (Block *b : stage->getBlockVec())
    {
        // Skip blocks that are outside of the screen
        // Set on screen state for less intensive collision checking
        if (int(b->getX()) + delta_x < -b->getGrid() || 
        scene->getHeight() - b->getGrid() - (int(b->getY()) + delta_y) > scene->getHeight() ||
        int(b->getX()) + delta_x > scene->getWidth() + b->getGrid() ||
        scene->getHeight() - b->getGrid() - (int(b->getY()) + delta_y) < -b->getGrid())
        {
            b->setOnScreen(false);
            continue;
        }
        else b->setOnScreen(true);
    
        des_rect = {
            int(b->getX()) + delta_x, 
            scene->getHeight() - b->getGrid() - (int(b->getY()) + delta_y), 
            b->getGrid(), 
            b->getGrid()
        };
        SDL_RenderCopy(scene->getRenderer(), b->getTexture(), NULL, &des_rect);
    }
}
void Renderer::renderMovingBlocks(Stage *stage, Player *player)
{
    for (Block *b : stage->getMovingBlockVec())
    {
        // Skip blocks that are outside of the screen
        if (int(b->getX()) + delta_x < -b->getGrid() || 
        scene->getHeight() - b->getGrid() - (int(b->getY()) + delta_y) > scene->getHeight() ||
        int(b->getX()) + delta_x > scene->getWidth() + b->getGrid() ||
        scene->getHeight() - b->getGrid() - (int(b->getY()) + delta_y) < -b->getGrid())
            continue;

        des_rect = {
            int(b->getX()) + delta_x, 
            scene->getHeight() - b->getGrid() - (int(b->getY()) + delta_y), 
            b->getGrid(), 
            b->getGrid()
        };
        SDL_RenderCopy(scene->getRenderer(), b->getTexture(), NULL, &des_rect);
    }
}
void Renderer::renderProjectiles(Stage *stage, Player *player)
{
    for (auto p : player->getProjectiles())
    {
        // Skip projectiles that are outside of the screen
        if (int(p->getX()) + delta_x < -p->getGrid() || 
        scene->getHeight() - p->getGrid() - (int(p->getY()) + delta_y) > scene->getHeight() ||
        int(p->getX()) + delta_x > scene->getWidth() + p->getGrid() ||
        scene->getHeight() - p->getGrid() - (int(p->getY()) + delta_y) < -p->getGrid())
            continue;

        des_rect = {
            int(p->getX()) + delta_x, 
            scene->getHeight() - p->getGrid() - (int(p->getY()) + delta_y), 
            p->getGrid(), 
            p->getGrid()
        };
        SDL_RenderCopy(scene->getRenderer(), p->getTexture(), NULL, &des_rect);
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
        renderGridLines(stage, player);
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

void Renderer::renderGridLines(Stage *stage, Player *player)
{
    // Render grid lines for level creation purposes
    SDL_SetRenderDrawColor(scene->getRenderer(), 255, 0, 0, 255);

    // With the player at the center, the grid array will start from that minus the screen width
    // and end at the player x + screen width
    int16_t array_start = (player->getX() - scene->getWidth()) / (player->getGrid() / 2);
    int16_t array_end = (player->getX() + scene->getWidth()) / (player->getGrid() / 2);
    for (int i = array_start; i < array_end; i++)
    {
        SDL_RenderDrawLine(scene->getRenderer(),
            i * (player->getGrid() / 2) + delta_x,
            0,
            i * (player->getGrid() / 2) + delta_x,
            scene->getHeight()
        );
    }
    // The same concept applies to vertical gridlines
    // In platforming mode, player has double the grid size of normal blocks
    // Therefore, it needs to be divided by 2 in order to get the right grid size
    array_start = (player->getY() - scene->getHeight()) / (player->getGrid() / 2);
    array_end = (player->getY() + scene->getHeight()) / (player->getGrid() / 2);
    for (int i = array_start; i < array_end; i++)
    {
        SDL_RenderDrawLine(scene->getRenderer(),
            0,
            scene->getHeight() - (i * (player->getGrid() / 2) + delta_y),
            scene->getWidth(),
            scene->getHeight() - (i * (player->getGrid() / 2) + delta_y)
        );
    }
    // Transparent color layer for better visibility
    SDL_SetRenderDrawBlendMode(scene->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(scene->getRenderer(), 192, 192, 192, 144);
    des_rect = {0, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderFillRect(scene->getRenderer(), &des_rect);

    // Player position highlight
    // Currently, the player character will act as the cursor for block placing
    uint16_t highlight_x = player->getCenterX() / (player->getGrid() / 2);
    uint16_t highlight_y = player->getCenterY() / (player->getGrid() / 2);
    SDL_SetRenderDrawBlendMode(scene->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(scene->getRenderer(), 255, 0, 0, 144);
    des_rect = {
        highlight_x * (player->getGrid() / 2) + delta_x, 
        scene->getHeight() - player->getGrid() / 2 - (highlight_y * (player->getGrid() / 2) + delta_y), 
        player->getGrid() / 2, 
        player->getGrid() / 2
    };
    SDL_RenderFillRect(scene->getRenderer(), &des_rect);

    // // Camera outline (for easier debugging)
    // SDL_SetRenderDrawColor(scene->getRenderer(), 255, 0, 0, 255);
    // // Horizontal lines
    // SDL_RenderDrawLine(scene->getRenderer(), 
    //     0,
    //     int(cam->getRendY()),
    //     scene->getWidth(),
    //     int(cam->getRendY())
    // );
    // SDL_RenderDrawLine(scene->getRenderer(), 
    //     0,
    //     int(cam->getRendY()) + cam->getGrid(),
    //     scene->getWidth(),
    //     int(cam->getRendY()) + cam->getGrid()
    // );
    // // Vertical lines
    // SDL_RenderDrawLine(scene->getRenderer(), 
    //     int(cam->getRendX()),
    //     0,
    //     int(cam->getRendX()),
    //     scene->getHeight()
    // );
    // SDL_RenderDrawLine(scene->getRenderer(), 
    //     int(cam->getRendX()) + cam->getGrid(),
    //     0,
    //     int(cam->getRendX()) + cam->getGrid(),
    //     scene->getHeight()
    // );
}
void Renderer::renderEditorMenu(Stage *stage, Editor *edit)
{

    // Menu background (solid color for now)
    uint16_t menu_width = scene->getWidth() / 3;
    SDL_SetRenderDrawBlendMode(scene->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(scene->getRenderer(), 220, 220, 220, 220);
    des_rect =
    {
        scene->getWidth() - menu_width,
        0,
        menu_width,
        scene->getHeight()
    };
    SDL_RenderFillRect(scene->getRenderer(), &des_rect);
    
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
            (scene->getWidth() - menu_width + edit->getGrid()) + (i % item_per_row * edit->getGrid() * 4 / 3),
            edit->getGrid() + ((i / item_per_row - row_offset) * edit->getGrid() * 4 / 3),
            edit->getGrid(),
            edit->getGrid()
        };
        SDL_RenderCopy(scene->getRenderer(), stage->getBlockTextures()[i], NULL, &des_rect);
    }

    // Menu buttons, menu blocks, highlight for selected block, etc.
    // Selected block highlight
    // std::cout << stage->getSEMCount() << "\n";
    SDL_SetRenderDrawBlendMode(scene->getRenderer(), SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(scene->getRenderer(), 255, 0, 0, 144);
    des_rect = 
    {
        (scene->getWidth() - menu_width + edit->getGrid()) + (current_column * edit->getGrid() * 4 / 3),
        edit->getGrid() + (current_row * edit->getGrid() * 4 / 3),
        edit->getGrid(),
        edit->getGrid()
    };
    SDL_RenderFillRect(scene->getRenderer(), &des_rect);
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

Renderer::~Renderer()
{
    std::cout << "Renderer terminated!\n";
}