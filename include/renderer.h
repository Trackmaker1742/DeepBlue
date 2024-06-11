#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "scene.h"
#include "object2d.h"
#include "camera.h"
#include "player.h"
#include "stage.h"

class Renderer 
{
private:
    // Player *player;
    Camera *cam;
    // Stage *stage;

    SDL_Window* window;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

public:
    Renderer(SDL_Window* w, Camera *c);

    // Getters
    SDL_Renderer *getRend();

    void initStage();

    void renderPlayer(Player *player);
    void renderStage(Stage *stage, Player *player);
};

#endif 