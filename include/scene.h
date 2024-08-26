#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "input.h"

class Scene
{
private:
    // Input pointer for easier reference
    // Not belonging to Scene
    Input *input;

    // Default values for game window and fps
    uint16_t width = 800;
    uint16_t height = 600;
    uint8_t fps = 60;
    uint8_t display_option = 0;

    SDL_Surface *icon = IMG_Load("res/icon.png");

    // Delta time calculation
    float delta_time;

    uint8_t current_scene = 0;

    SDL_Window *window;
    SDL_Renderer *renderer;

    // Game state
    uint8_t game_state = 1; // 0: Quit game
                            // 1: Main menu (new game, continue, stage select, gallery, settings, quit game)
                            // 2: Stage select menu (read from save file to see if stage is played at least once)
                            // 3: Gallery (item collection status, unlockables?)
                            // 4: Settings (write stuff into config, reload)
                            // 5: Pause menu (resume, settings, quit to menu)

                            // 6: Platforming stages (1, 2, 4, 6, 8)
                            // 7: Vertical shooting stage (3)
                            // 8: Horizontal shooting stage (5)
                            // 9: Rhythm stage (7)
                            // 10: Cutscenes (disable controls)

    uint8_t menu_counter = 0;   // Handle menu pointer position
                                // Reset to 0 for each menu switch

    // I'll need an array for each of these, huh

    std::vector<SDL_Texture*> bg_array; // 0: Main
                                        // 1: Stage
                                        // 2: Setting
                                        // 3: Gallery
                                        // 4: Pause

public:
    Scene(Input *in);

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
    SDL_Texture *getBackground(int i);    // Proper indexing for each bg I guess

    // Init Menu
    void initMenu(const char *path);

    void initAllMenu();

    // Update Menu
    void updateMainMenu();
    void updateStageSelect();
    void updateSettings();
    void updateGallery();
    void updatePauseMenu();

    ~Scene();

};

#endif