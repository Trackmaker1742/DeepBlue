#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Scene
{
private:
    // Default values for game window and fps
    uint16_t width = 1024;
    uint16_t height = 640;
    uint8_t fps = 60;
    uint8_t display_option = 0; 

    // Delta time calculation
    float delta_time;

    uint8_t current_scene = 0;

    SDL_Window *window;
    SDL_Renderer *renderer;

    // Background
    SDL_Texture* bg;

public:
    // (Re-)Initialize the game window
    void init(uint16_t w, uint16_t h, uint8_t f, uint8_t dopt);
    void update(uint16_t w, uint16_t h, uint8_t f, uint8_t dopt);

    // Setters
    void setDeltaTime(float dt);

    // Getters
    uint16_t getWidth();
    uint16_t getHeight();
    uint8_t getFPS();
    float getDeltaTime();
    SDL_Window *getWindow();
    SDL_Renderer *getRenderer();
};

#endif