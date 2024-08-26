#include "scene.h"

Scene::Scene(Input *in) : input(in)
{ }

void Scene::init(uint16_t w, uint16_t h, uint8_t f, uint8_t dopt)
{
    // Update the window values
    width = w;
    height = h;
    fps = f;
    display_option = dopt;
    // Switch case for display option (later)
    window = SDL_CreateWindow("Deep Blue", 0, 0, width, height, SDL_WINDOW_BORDERLESS & SDL_WINDOW_POPUP_MENU);
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
void Scene::setState(uint8_t st) { game_state = st; }

// Getters
uint16_t Scene::getWidth() { return width; }
uint16_t Scene::getHeight() { return height; }
uint8_t Scene::getFPS() { return fps; }
float Scene::getDeltaTime() { return delta_time; }
SDL_Window *Scene::getWindow() { return window; }
SDL_Renderer *Scene::getRenderer() { return renderer; }
uint8_t Scene::getState() { return game_state; }
SDL_Texture *Scene::getBackground(int i) { return bg_array[i];}

// Menu stuff
void Scene::initMenu(const char *path)
{
    // Static background (for now)
    SDL_Surface* surface = IMG_Load(path);
    bg_array.push_back(SDL_CreateTextureFromSurface(renderer, surface));
    SDL_FreeSurface(surface);
}

void Scene::initAllMenu()
{
    initMenu("res/Menus/Main Menu.png");
    initMenu("res/Menus/Stage Select.png");
    initMenu("res/Menus/Settings.png");
    initMenu("res/Menus/Gallery.png");
    initMenu("res/Menus/Pause Menu.png");
}

// Update Menu (mainly input, render in renderer please!)
void Scene::updateMainMenu()
{
    // Navigation (top to bottom)
    if (input->getPress(0) && menu_counter > 0)
    {
        input->setHold(0, false);
        menu_counter--;
    }
    if (input->getPress(1) && menu_counter < 4)
    {
        input->setHold(1, false);
        menu_counter++;
    }
    // Choose the highlighted option
    if (input->getPress(4))
    {
        input->setHold(4, false);
        menu_counter = 0;
        switch (menu_counter)
        {
            case 0: // New game
                // game_state = ;
            break;
            case 1: // Stage select
                // game_state = 
            break;
            case 2: // Settings
            
            break;
            case 3: // Gallery
            
            break;
            case 4: // Quit game
            
            break;
            default:
            break;
        }
    }
}

void Scene::updateStageSelect()
{
    // Will need more condition, 
    // since you can only pick stages after unlocking them
    // Navigation (left to right)
    if (input->getPress(2) && menu_counter > 0)
    {
        input->setHold(2, false);
        menu_counter--;
    }
    if (input->getPress(3) && menu_counter < 7)
    {
        input->setHold(3, false);
        menu_counter++;
    }
    // Choose the highlighted option
    if (input->getPress(4))
    {
        input->setHold(4, false);
        menu_counter = 0;
    }  
}

void Scene::updateSettings()
{
    // This one is gonna be painful
    // I'm thinking of limiting it to just config settings
    // Allowing for input mapping would require a full input rework
    // Exit this will write into config.txt
    // Exit and save setting prompt
}

void Scene::updateGallery()
{
    // 6x5 grid, totalling up to 30
    //  0  1  2  3  4  5
    //  6  7  8  9 10 11
    // 12 13 14 15 16 17
    // 18 19 20 21 22 23
    // 24 25 26 27 28 29
    // Final item position tbd
    
    // Navigation (all 4 directions)
    if (input->getPress(0) && menu_counter > 5)
    {
        input->setHold(0, false);
        menu_counter -= 6;
    }
    if (input->getPress(1) && menu_counter < 24)
    {
        input->setHold(1, false);
        menu_counter += 6;
    }
    if (input->getPress(2) && menu_counter > 0)
    {
        input->setHold(2, false);
        menu_counter--;
    }
    if (input->getPress(3) && menu_counter < 29)
    {
        input->setHold(3, false);
        menu_counter++;
    }
    // Choose the highlighted option
    // Might have a desc zone and hover only instead
    // No need for clicking
    if (input->getPress(4))
    {
        input->setHold(4, false);
        // Zoom into item, add description
    }
}

void Scene::updatePauseMenu()
{
    // Navigation (top to bottom)
    if (input->getPress(0) && menu_counter > 0)
    {
        input->setHold(0, false);
        menu_counter--;
    }
    if (input->getPress(1) && menu_counter < 2)
    {
        input->setHold(1, false);
        menu_counter++;
    }
    // Choose the highlighted option
    if (input->getPress(4))
    {
        input->setHold(4, false);
        menu_counter = 0;
        switch (menu_counter)
        {
            case 0: // Resume
                // game_state = ;
            break;
            case 1: // Settings
                // game_state = 
            break;
            case 2: // Quit to menu
            
            break;
            default:
            break;
        }
    }
}

Scene::~Scene()
{ 
    bg_array.clear();
}