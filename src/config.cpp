#define WINVER 0x0605
#define _WIN32_WINNT 0x0605

#include "config.h"

Config::Config(Input *in) : input(in)
{ }

void Config::init(uint16_t w, uint16_t h, uint8_t dopt, uint16_t f)
{
    // Update the window values
    width = w;
    height = h;
    display_option = dopt;
    fps = f;

    // Get the display mode for primary display
    SDL_GetDesktopDisplayMode(0, &display_mode);

    SetProcessDPIAware(); // Used to avoid Windows DPI scaling

    // Switch case for display option
    uint32_t flag;
    switch (display_option)
    {
        case 0:
            // Windowed mode with specified dimensions
            window = SDL_CreateWindow("Deep Blue", 0, 0, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); // Center the window
            break;
        case 1:
            // Borderless fullscreen mode, use display resolution
            width = display_mode.w;
            height = display_mode.h;
            window = SDL_CreateWindow("Deep Blue", 0, 0, width, height, SDL_WINDOW_FULLSCREEN_DESKTOP);
            break;
        case 2:
            // Exclusive fullscreen mode, use display resolution
            width = display_mode.w;
            height = display_mode.h;
            window = SDL_CreateWindow("Deep Blue", 0, 0, width, height, SDL_WINDOW_FULLSCREEN);
            break;
        default:
            break;
    }

    scale_factor = float(width) / 1920;
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetWindowIcon(window, icon);
}

void Config::update(uint16_t w, uint16_t h, uint8_t dopt, uint16_t f)
{
    // Update the window values
    width = w;
    height = h;
    display_option = dopt;
    fps = f;

    // Get the display mode for primary display
    SDL_GetDesktopDisplayMode(0, &display_mode);

    // Switch case for display option
    switch (display_option)
    {
        // Windowed mode
        case 0:
            SDL_SetWindowFullscreen(window, 0);  // Exit fullscreen mode
            SDL_SetWindowSize(window, width, height);  // Set to specified windowed size
            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED); // Center the window
            break;
        // Borderless fullscreen mode, adjust window size to display resolution
        case 1:
            width = display_mode.w;
            height = display_mode.h;
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            SDL_SetWindowSize(window, width, height); // Update window size
            break;
        // Exclusive fullscreen mode, adjust window size to display resolution
        case 2:
            width = display_mode.w;
            height = display_mode.h;
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            SDL_SetWindowSize(window, width, height); // Update window size
            break;
        default:
            break;
    }

    scale_factor = float(width) / 1920;
}

// Setters
void Config::setDeltaTime(float dt) { delta_time = dt; }
void Config::setRenderer(SDL_Renderer *r) { renderer = r; }

// Getters
uint16_t Config::getWidth() { return width; }
uint16_t Config::getHeight() { return height; }
uint8_t Config::getFPS() { return fps; }
float Config::getDeltaTime() { return delta_time; }
SDL_Window *Config::getWindow() { return window; }
SDL_Renderer *Config::getRenderer() { return renderer; }
uint8_t Config::getDGrid() { return d_grid; }
float Config::getScaleFactor() { return scale_factor; }

Config::~Config()
{ 
    std::cout << "Config terminated!\n";
}