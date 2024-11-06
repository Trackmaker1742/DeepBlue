#ifndef CONFIG_H
#define CONFIG_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <windows.h>

#include "input.h"

class Config
{
private:
    // Input pointer for easier reference
    // Not belonging to Config
    Input *input;

    // Default values for game window and fps
    uint16_t width = 0;
    uint16_t height = 0;
    uint8_t display_option = 0;
    uint8_t fps = 0;

    SDL_DisplayMode display_mode;

    SDL_Surface *icon = IMG_Load("res/icon.png");

    // Delta time calculation
    float delta_time;

    uint8_t current_config = 0;

    SDL_Window *window;
    SDL_Renderer *renderer;
    
    uint8_t menu_counter = 0;   // Handle menu pointer position
                                // Reset to 0 for each menu switch

public:
    Config(Input *in);

    // (Re-)Initialize the game window
    void init(uint16_t w, uint16_t h, uint8_t f, uint8_t dopt);
    void update(uint16_t w, uint16_t h, uint8_t f, uint8_t dopt);

    // Setters
    void setDeltaTime(float dt);
    void setState(uint8_t st);

    // Getters
    uint16_t getWidth();
    uint16_t getHeight();
    uint8_t getFPS();
    float getDeltaTime();
    SDL_Window *getWindow();
    SDL_Renderer *getRenderer();
    uint8_t getState();
    
    ~Config();
};

#endif