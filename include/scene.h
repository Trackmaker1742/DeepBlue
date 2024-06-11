#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Scene
{
private:
    // Delta time calculation
    float delta_time;

    uint8_t current_scene = 0;

    SDL_Window* window = SDL_CreateWindow("Deep Blue", 0, 30, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    // Background
    SDL_Texture* bg;

public:
    static const uint16_t WIDTH = 960;
    static const uint16_t HEIGHT = 512;

    static const uint8_t FPS = 120;

    // Setters
    void setDeltaTime(float dt);

    // Getters
    float getDeltaTime();
    SDL_Window* getWindow();
};

#endif