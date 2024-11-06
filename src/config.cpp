#define WINVER 0x0605
#define _WIN32_WINNT 0x0605

#include "config.h"

Config::Config(Input *in) : input(in)
{ }

void Config::init(uint16_t w, uint16_t h, uint8_t dopt, uint8_t f)
{
    // Update the window values
    width = w;
    height = h;
    display_option = dopt;
    fps = f;

    SDL_GetDesktopDisplayMode(0, &display_mode);

    SetProcessDPIAware();   // Used to avoid Windows DPI scaling

    // Switch case for display option (later)
    uint32_t flag;
    switch (display_option)
    {
        case 0:
            window = SDL_CreateWindow("Deep Blue", 0, 50, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
        break;
        case 1:
            // width = display_mode.w;
            // height = display_mode.h;
            window = SDL_CreateWindow("Deep Blue", 0, 0, width, height, SDL_WINDOW_FULLSCREEN_DESKTOP);
        break;
        case 2:
            // width = display_mode.w;
            // height = display_mode.h;
            window = SDL_CreateWindow("Deep Blue", 0, 0, width, height, SDL_WINDOW_FULLSCREEN);
        break;
        default:
        break;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetWindowIcon(window, icon);
}
void Config::update(uint16_t w, uint16_t h, uint8_t dopt, uint8_t f)
{
    // Update the window values
    width = w;
    height = h;
    display_option = dopt;
    fps = f;
    // Switch case for display option (later)
    switch (display_option)
    {
        // Windowed
        case 0:
            SDL_SetWindowSize(window, width, height);
        break;
        // Borderless windowed (fullscreen size)
        case 1:
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        break;
        // Fullscreen
        case 2:
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        break;
        default:
        break;
    }
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