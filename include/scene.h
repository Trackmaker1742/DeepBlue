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
    PAUSE
};

enum class Dir
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class Settings
{
    RESOLUTION,
    DISPLAY_OPTION,
    FRAMERATE
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
    uint8_t pre_pause_state;

    uint8_t menu_counter = 0;

    uint8_t stage_number = 0;   // + 48 to convert from int to coresponding char

    std::vector<uint8_t> setting_counters =
    {
        0,
        0,
        0
    };
    uint8_t setting_counter_max = 0;
    std::vector<std::string> preset_resolution =
    {
        "1280 x 720",
        "1280 x 800",
        
        "1920 x 1080",
        "1920 x 1200",
        
        "2560 x 1440",
        "2560 x 1600",

        "2880 x 1620",
        "2880 x 1800",
        
        "3840 x 2160",
        "3840 x 2400",
    };
    std::vector<std::string> preset_display_option =
    {
        "Windowed",
        "Borderless Windowed",
        "Fullscreen"
    };
    std::vector<std::string> preset_framerate =
    {
        "30",
        "60",
        "75",
        "90",
        "120",
        "144",
        "240"
    };

    std::string temp_reso_str = "";

public:
    Scene(SDL_Renderer *rend);

    // Getters
    SDL_Texture *getBgTexture(MenuIndex m_index, int index);
    uint8_t getState();
    uint8_t getPrevState();
    uint8_t getPrePauseState();
    uint8_t getCounter();
    uint8_t getStageNum();
    uint8_t getSettingCounter(Settings setting);
    std::vector<std::string> getPresetArray(Settings setting);

    // Setters
    void setState(uint8_t st);

    void backButton(Input *input);
    bool press(Dir direction, Input *input);

    // Init Scene
    void initSettingValues(uint16_t w, uint16_t h, uint8_t dopt, uint16_t f);

    void initMenuTextures(File_Handler *file);
    
    void initStage(Config *config, File_Handler *file, 
        Stage *stage, Player *player);

    void pauseHandler(Input *input);

    // Update Scene
    void updateMain(Input *input);
    void updateStageSelect(Input *input, Config *config, 
        File_Handler *file, Stage *stage, Player *player);
    void updateSettings(Input *input, Config *config, 
        File_Handler *file, Stage *stage, Player *player);
    void updateGallery(Input *input);
    void updatePause(Input *input, Stage *stage, Player *player);
};

#endif