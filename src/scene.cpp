#include "scene.h"

void Scene::init(uint16_t w, uint16_t h, uint8_t f, uint8_t dopt)
{
    // Update the window values
    width = w;
    height = h;
    fps = f;
    display_option = dopt;
    // Switch case for display option (later)
    window = SDL_CreateWindow("Deep Blue", 0, 30, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Scene::update(uint16_t w, uint16_t h, uint8_t f, uint8_t dopt)
{
    // Update the window values
    width = w;
    height = h;
    fps = f;
    display_option = dopt;
    // Switch case for display option (later)
    SDL_SetWindowSize(window, width, height);
}

// Setters
void Scene::setDeltaTime(float dt) { delta_time = dt; }

// Getters
uint16_t Scene::getWidth() { return width; }
uint16_t Scene::getHeight() { return height; }
uint8_t Scene::getFPS() { return fps; }
float Scene::getDeltaTime() { return delta_time; }
SDL_Window *Scene::getWindow() { return window; }
SDL_Renderer *Scene::getRenderer() { return renderer; }