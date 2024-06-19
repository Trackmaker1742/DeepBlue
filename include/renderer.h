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

    Scene *scene;

public:
    Renderer(Scene *sc, Camera *c);

    void initStage();

    void renderPlayer(Player *player);
    void renderStage(Stage *stage, Player *player);
};

#endif 