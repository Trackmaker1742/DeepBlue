#define WINVER 0x0605
#define _WIN32_WINNT 0x0605

#include "config.h"

Config::Config(Input *in) : input(in)
{ }

void Config::init(uint16_t w, uint16_t h, uint8_t f, uint8_t dopt)
{
    // Update the window values
    width = w;
    height = h;
    fps = f;
    display_option = dopt;
    // Switch case for display option (later)
    SetProcessDPIAware();   // Used to avoid Windows DPI scaling
    window = SDL_CreateWindow("Deep Blue", 0, 50, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetWindowIcon(window, icon);
}

void Config::update(uint16_t w, uint16_t h, uint8_t f, uint8_t dopt)
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
void Config::setDeltaTime(float dt) { delta_time = dt; }

// Getters
uint16_t Config::getWidth() { return width; }
uint16_t Config::getHeight() { return height; }
uint8_t Config::getFPS() { return fps; }
float Config::getDeltaTime() { return delta_time; }
SDL_Window *Config::getWindow() { return window; }
SDL_Renderer *Config::getRenderer() { return renderer; }

Config::~Config()
{ 
    std::cout << "Config terminated!\n";
}