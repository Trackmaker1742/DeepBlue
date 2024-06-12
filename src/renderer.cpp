#include "renderer.h"

Renderer::Renderer(SDL_Window *w, Camera *c) : 
    window(w),
    // player(p), 
    cam(c)
    // stage(stg)
{ }

SDL_Renderer *Renderer::getRend()
{
    return renderer;
}

void Renderer::renderPlayer(Player *player)
{
    // Values for offsetting the camera from the player when in the edges of the stage
    int16_t p_cam_offset_x = player->getX() - cam->getX();
    int16_t p_cam_offset_y = player->getY() - cam->getY();

    SDL_Rect des = {
        cam->getRendX() + p_cam_offset_x, 
        Scene::HEIGHT - player->getGrid() - cam->getRendY() - p_cam_offset_y, 
        player->getGrid(), 
        player->getGrid()
    };
    SDL_RenderCopy(renderer, player->getTexture(), NULL, &des);
}

void Renderer::renderStage(Stage *stage, Player *player)
{
    // Render background
    SDL_Rect des = {0, 0, Scene::WIDTH, Scene::HEIGHT};
    SDL_RenderCopy(renderer, stage->getBackground(), NULL, &des);
    
    int16_t delta_x = cam->getRendX() - cam->getX();
    int16_t delta_y = cam->getRendY() - cam->getY();

    // Render blocks
    for (Block* b : stage->getBlockVec())
    { 
        SDL_Rect des_rect = {
            int(b->getGridX()) + delta_x, 
            Scene::HEIGHT - b->getGrid() - (int(b->getGridY()) + delta_y), 
            b->getGrid(), 
            b->getGrid()
        };
        SDL_RenderCopy(renderer, b->getTexture(), NULL, &des_rect);
    }

    // Render player
    renderPlayer(player);
}

// void Renderer::renderPlayer(Player *player)
// {
//     SDL_Rect des = {int(player->getX()), int(Scene::HEIGHT - player->getY()) - player->getGrid(), player->getGrid(), player->getGrid()};
//     SDL_RenderCopy(renderer, player->getTexture(), NULL, &des);
// }

// void Renderer::renderStage(Stage *stage, Player *player)
// {
//     // Render background
//     SDL_Rect des = {0, 0, Scene::WIDTH, Scene::HEIGHT};
//     SDL_RenderCopy(renderer, stage->getBackground(), NULL, &des);
    
//     // Render blocks
//     for (Block* b : stage->getBlockVec())
//     { 
//         SDL_Rect des_rect = {int(b->getGridX()), Scene::HEIGHT - int(b->getGridY()) - b->getGrid(), b->getGrid(), b->getGrid()};
//         SDL_RenderCopy(renderer, b->getTexture(), NULL, &des_rect);
//     }

//     // Render player
//     renderPlayer(player);
// }