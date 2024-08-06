#include "renderer.h"

Renderer::Renderer(Scene *sc, Camera *c) : 
    scene(sc),
    // player(p), 
    cam(c)
    // stage(stg)
{ }

void Renderer::renderPlayer(Player *player)
{
    // Values for offsetting the camera from the player when in the edges of the stage
    int16_t p_cam_offset_x = player->getX() - cam->getX();
    int16_t p_cam_offset_y = player->getY() - cam->getY();

    SDL_Rect des = {
        cam->getRendX() + p_cam_offset_x, 
        scene->getHeight() - player->getGrid() - cam->getRendY() - p_cam_offset_y, 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderCopy(scene->getRenderer(), player->getTexture(), NULL, &des);
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
    SDL_Rect des = {des1, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), stage->getBackground(), NULL, &des);
    des = {des2, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), stage->getBackground(), NULL, &des);
}

void Renderer::renderStage(Stage *stage, Player *player)
{
    // Render background
    renderBackground(stage, player);

    // Camera stuff for other renders
    int16_t delta_x = cam->getRendX() - cam->getX();
    int16_t delta_y = cam->getRendY() - cam->getY();

    // Render blocks
    for (Block* b : stage->getBlockVec())
    { 
        SDL_Rect des_rect = {
            int(b->getGridX()) + delta_x, 
            scene->getHeight() - b->getGrid() - (int(b->getGridY()) + delta_y), 
            b->getGrid(), 
            b->getGrid()
        };
        SDL_RenderCopy(scene->getRenderer(), b->getTexture(), NULL, &des_rect);
    }

    // Render player
    renderPlayer(player);
}

// Menu render has to be run here to keep consistency
// And moving bg is pretty long anyway
// Separating input from render is a good practice as well
void Renderer::renderMainMenu()
{
    // Render background
    SDL_Rect des = {0, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), scene->getBackground(0), NULL, &des);
}

void Renderer::renderStageSelect()
{
    // Render background
    SDL_Rect des = {0, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), scene->getBackground(1), NULL, &des);
}

void Renderer::renderGallery()
{
    // Render background
    SDL_Rect des = {0, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), scene->getBackground(2), NULL, &des);
}

void Renderer::renderSettings()
{
    // Render background
    SDL_Rect des = {0, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), scene->getBackground(3), NULL, &des);
}

void Renderer::renderPauseMenu()
{
    // Render background
    SDL_Rect des = {0, 0, scene->getWidth(), scene->getHeight()};
    SDL_RenderCopy(scene->getRenderer(), scene->getBackground(4), NULL, &des);
}