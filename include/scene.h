#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>

#include "config.h"
#include "input.h"
#include "file_handler.h"
#include "stage.h"
#include "player.h"

enum class MenuIndex
{
    MAIN,
    STAGESELECT,
    GALLERY,
    SETTINGS,
    PAUSE,

    INDEX_MAX
};

class Scene
{
private:
    SDL_Renderer *renderer;
    // String matrix containing an arrray of paths for each menu
    // Index list
    // 0: Main
    // 1: Stage Select
    // 2: Gallery
    // 3: Settings
    // 4: Pause
    std::vector<std::vector<std::string>> bg_paths;
    
    std::vector<std::vector<SDL_Texture*>> bg_texture_matrix;

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
                            // 10: Cutscene (disable controls)
    uint8_t prev_state;

    uint8_t menu_counter = 0;

    uint8_t stage_number = 0;   // + 48 to convert from int to coresponding char

public:
    Scene(SDL_Renderer *rend);

    // Getters
    SDL_Texture *getBgTexture(MenuIndex m_index, int index);
    uint8_t getState();
    uint8_t getCounter();
    uint8_t getStageNum();

    // Setters
    void setState(uint8_t st);

    void backButton(Input *input);

    // Init Scene
    void initMenuTextures(File_Handler *file);
    
    void initStage(Config *config, File_Handler *file, 
        Stage *stage, Player *player);

    void pauseHandler(Input *input);

    // Update Scene
    void updateMain(Input *input);
    void updateStageSelect(Input *input, Config *config, 
        File_Handler *file, Stage *stage, Player *player);
    void updateSettings(Input *input);
    void updateGallery(Input *input);
    void updatePause(Input *input, Stage *stage, Player *player);
};

#endif