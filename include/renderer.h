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

    bool init_x = false;    // Flag for initializing the initial position (for moving bg)
    uint16_t initial_x;    // Initial x position of player

    SDL_Rect src_rect;
    SDL_Rect des_rect;

    // Rendering variable
    float idle_counter = 0;
    float run_counter = 0;

    float looped_counter = 0;   // Ascend, descend

    float temp_counter = 0;     // Jump_start, apex, land

public:
    Renderer(Scene *sc, Camera *c);

    void renderPlayer(Player *player, float dt);
    void renderBackground(Stage *stage, Player *player);
    void renderStage(Stage *stage, Player *player);

    void renderMainMenu();
    void renderStageSelect();
    void renderGallery();
    void renderSettings();
    void renderPauseMenu();
};

#endif 