#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "scene.h"
#include "object2d.h"
#include "camera.h"
#include "player.h"
#include "stage.h"
#include "effect.h"

class Renderer 
{
private:
    // Player *player;
    Camera *cam;
    // Stage *stage;

    Scene *scene;

    bool init_x = false;    // Flag for initializing the initial position (for moving bg)
    uint16_t initial_x;    // Initial x position of player

    // Rect for sprite handling
    SDL_Rect src_rect;
    SDL_Rect des_rect;

    // Rect for effects handling
    SDL_Rect src_rect_d;
    SDL_Rect des_rect_d;
    SDL_Rect src_rect_j;
    SDL_Rect des_rect_j;

    // Rendering variable
    float idle_counter = 0;
    float run_counter = 0;

    float looped_counter = 0;   // Ascend, descend

    float temp_counter = 0;     // Jump_start, apex, land

    // For ancoring the effect coordinates
    uint16_t player_dash_x = 0;
    uint16_t player_dash_y = 0;
    bool player_temp_right = false;
    bool player_temp_on_ground = false;
    bool dash_anchor = false;
    float dash_effect_counter = 0;

    uint16_t player_jump_x = 0;
    uint16_t player_jump_y = 0;
    bool jump_anchor = false;
    float jump_effect_counter = 0;

public:
    Renderer(Scene *sc, Camera *c);

    void renderPlatformer(Player *player);
    void renderVertShooter(Player *player);
    void renderHoriShooter(Player *player);
    void renderRhythm(Player *player);

    void renderBackground(Stage *stage, Player *player);
    void renderStage(Stage *stage, Player *player);

    void renderMainMenu();
    void renderStageSelect();
    void renderGallery();
    void renderSettings();
    void renderPauseMenu();
};

#endif 