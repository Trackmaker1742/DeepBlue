#include "scene.h"

Scene::Scene(SDL_Renderer *rend) : renderer(rend)
{ }

// Getters
SDL_Texture *Scene::getBgTexture(MenuIndex m_index, int index) { return bg_texture_matrix[int(m_index)][index]; }
uint8_t Scene::getState() { return game_state; }
uint8_t Scene::getPrevState() { return prev_state; }
uint8_t Scene::getPrePauseState() { return pre_pause_state; }
uint8_t Scene::getCounter() { return menu_counter; }
uint8_t Scene::getStageNum() { return stage_number; }
uint8_t Scene::getSettingCounter(Settings setting) { return setting_counters[static_cast<int>(setting)]; }
std::vector<std::string> Scene::getPresetArray(Settings setting)
{
    switch (setting)
    {
        case Settings::RESOLUTION:
            return preset_resolution;
        break;
        case Settings::DISPLAY_OPTION:
            return preset_display_option;
        break;
        case Settings::FRAMERATE:
            return preset_framerate;
        break;
        default:
        break;
    }
}

// Setters
void Scene::setState(uint8_t st) { game_state = st; }

void Scene::backButton(Input *input)
{
    uint8_t temp;
    // Back button
    if (input->getPress(Action::ACTION2))
    {
        temp = game_state;
        game_state = prev_state; // Go back to previous state
        prev_state = temp;
        menu_counter = 0;
    }
}

bool Scene::press(Dir direction, Input *input)
{
    switch (direction)
    {
        case Dir::UP:
            if (input->getPress(Action::MOVE_UP) ||
                input->getPress(Action::EXTRA_UP))
                return true;
        break;
        case Dir::DOWN:
            if (input->getPress(Action::MOVE_DOWN) ||
                input->getPress(Action::EXTRA_DOWN))
                return true;
        break;
        case Dir::LEFT:
            if (input->getPress(Action::MOVE_LEFT) ||
                input->getPress(Action::EXTRA_LEFT))
                return true;
        break;
        case Dir::RIGHT:
            if (input->getPress(Action::MOVE_RIGHT) ||
                input->getPress(Action::EXTRA_RIGHT))
                return true;
        break;
        default:
        break;
    }
    return false;
}

// Scene stuff
void Scene::initSettingValues(uint16_t w, uint16_t h, uint8_t dopt, uint16_t f)
{
    temp_reso_str = std::to_string(w) + " x " + std::to_string(h);
    
    // Set counter to match the value in the file
    for (int i = 0; i < preset_resolution.size(); i++)
    {
        if (temp_reso_str == preset_resolution[i])
        {
            setting_counters[0] = i;
            break;
        }
    }
    for (int i = 0; i < preset_display_option.size(); i++)
    {
        if (dopt == i)
        {
            setting_counters[1] = i;
            break;
        }
    }
    for (int i = 0; i < preset_framerate.size(); i++)
    {
        if (std::to_string(f) == preset_framerate[i])
        {
            setting_counters[2] = i;
            break;
        }
    }
    // std::cout << int(setting_counters[0]) << " " 
    //     << int(setting_counters[1]) << " " 
    //     << int(setting_counters[2]) << "\n";
}

void Scene::initMenuTextures(File_Handler *file)
{
    // Read assets and save file paths into a string matrix
    file->readMenuBgAsset(bg_paths);

    // Iterate through the string matrix to create texture, then save into a texture array
    for (std::vector<std::string> bg_paths_array : bg_paths)
    {
        std::vector<SDL_Texture*> temp_texture_array;
        for(std::string path : bg_paths_array)
        {
            temp_texture_array.push_back(IMG_LoadTexture(renderer, path.c_str()));
            SDL_SetTextureBlendMode(temp_texture_array.back(), SDL_BLENDMODE_BLEND);
        }
        bg_texture_matrix.push_back(temp_texture_array);
    }
}

void Scene::initStage(Config *config, 
    File_Handler *file, Stage *stage, Player *player, Camera *cam)
{
    // Change game state (manual for now)
    switch (stage_number)
    {
        // Platforming init
        case 1: case 2: case 4: case 6: case 8:
            stage->initPlatAll(file, stage_number + 48);
            player->initPlat(config);
            // Set spawn
            player->setX(stage->getRespX());
            player->setY(stage->getRespY());
            cam->init(config);
            game_state = 6;
        break;
        // Vertical shooter init
        case 3:
            stage->initVertShooterAll(file, stage_number + 48);
            player->initVertShooter(config);
            // Set spawn
            player->setX(config->getWidth() / 2);
            player->setY(config->getHeight() / 2);
            game_state = 7;
        break;
        // Horizontal shooter init
        case 5:
            stage->initHoriShooterAll(file, stage_number + 48);
            player->initHoriShooter(config);
            // Set spawn
            player->setX(config->getWidth() / 2);
            player->setY(config->getHeight() / 2);
            game_state = 8;
        break;
        // Rhythm init
        case 7:
            // Stage init rhythm all
            player->initRhythm(config);
            game_state = 9;
        break;
        default:
        break;
    }
}

void Scene::pauseHandler(Input *input)
{
    // Input for entering pause menu
    if (input->getPress(Action::PAUSE))
    {
        prev_state = game_state;
        pre_pause_state = prev_state;
        game_state = 5;
        menu_counter = 0;
    }
}

// Update Scene (mainly input, render in renderer please!)
void Scene::updateMain(Input *input)
{
    // Navigation (top to bottom)
    if (press(Dir::UP, input) && 
        menu_counter > 0)
    {
        menu_counter--;
    }
    if (press(Dir::DOWN, input) && 
        menu_counter < 4)
    {
        menu_counter++;
    }
    // Choose the highlighted option
    if (input->getPress(Action::ACTION1))
    {
        switch (menu_counter)
        {
            case 0: // New game
                // game_state = ;
                // TBD
            break;
            case 1: // Stage select
                prev_state = game_state;
                game_state = 2;
                menu_counter = 0;
            break;
            case 2: // Gallery
                prev_state = game_state;
                // TBD
            break;
            case 3: // Settings
                prev_state = game_state;
                game_state = 4;
                menu_counter = 0;
            break;
            case 4: // Quit game
                game_state = 0;
            break;
            default:
            break;
        }
    }
}

void Scene::updateStageSelect(Input *input, Config *config, 
    File_Handler *file, Stage *stage, Player *player, Camera *cam)
{
    // Will need more condition, 
    // since you can only pick stages after unlocking them
    // Navigation (left to right)
    if (press(Dir::LEFT, input) && 
        menu_counter > 0)
    {
        menu_counter--;
    }
    if (press(Dir::RIGHT, input) && 
        menu_counter < 8)
    {
        menu_counter++;
    }
    // Choose the highlighted option
    if (input->getPress(Action::ACTION1))
    {
        stage_number = menu_counter + 1;    // For loading the correct stage
        initStage(config, file, stage, player, cam);
        menu_counter = 0;
    }
    // Back button
    backButton(input);
}

void Scene::updateSettings(Input *input, Config *config,
    File_Handler *file, Stage *stage, Player *player, Camera *cam)
{
    // Navigation (top to bottom)
    if (press(Dir::UP, input) && 
        menu_counter > 0 && menu_counter != 4)
    {
        menu_counter--;
    }
    if (press(Dir::DOWN, input) && 
        menu_counter < 3)
    {
        menu_counter++;
    }
    // Change resolution, display option, framerate based on preset values
    if (menu_counter >= 0 && menu_counter < 3)
    {
        switch (menu_counter)
        {
            case static_cast<int>(Settings::RESOLUTION):
                setting_counter_max = preset_resolution.size();
            break;
            case static_cast<int>(Settings::DISPLAY_OPTION):
                setting_counter_max = preset_display_option.size();
            break;
            case static_cast<int>(Settings::FRAMERATE):
                setting_counter_max = preset_framerate.size();
            break;
            default:
            break;
        }
        if (press(Dir::LEFT, input) &&
            setting_counters[menu_counter] > 0)
        {
            setting_counters[menu_counter]--;
        }
        if (press(Dir::RIGHT, input) &&
            setting_counters[menu_counter] < setting_counter_max - 1)
        {
            setting_counters[menu_counter]++;
        }
    }
    
    // Press apply new settings
    if (input->getPress(Action::ACTION1) && menu_counter == 3)
    {
        SDL_SetWindowFullscreen(config->getWindow(), 0);
        // Update the game window
        config->update(file->getValue(0), 
            file->getValue(1), 
            file->getValue(2), 
            file->getValue(3));

        // Write new settings into file
        // If borderless fullscreen or fullscreen
        if (setting_counters[1] > 0)
        {
            temp_reso_str = std::to_string(config->getWidth()) + " x " + std::to_string(config->getHeight());
            for (int i = 0; i < preset_resolution.size(); i++)
            {
                if (temp_reso_str == preset_resolution[i])
                {
                    setting_counters[0] = i;
                    break;
                }
            }
            file->writeSave(
                temp_reso_str,
                setting_counters[1],
                preset_framerate[setting_counters[2]]
            );
        }
        else
        {
            file->writeSave(
                preset_resolution[setting_counters[0]],
                setting_counters[1],
                preset_framerate[setting_counters[2]]
            );
        }
        
        cam->init(config);
        stage->updateScaleFactor(config->getScaleFactor());
        
        // Update scaling for each game mode using the pause menu
        if (prev_state == 5)
        {
            // Player
            switch (pre_pause_state)
            {
                case 6:
                    player->setX(player->getX() / player->getGrid());
                    player->setY(player->getY() / player->getGrid());
                    player->initPlat(config);
                    player->setX(player->getX() * player->getGrid());
                    player->setY(player->getY() * player->getGrid());
                break;
                case 7:
                    player->setX(player->getX() / player->getGrid());
                    player->setY(player->getY() / player->getGrid());
                    player->initVertShooter(config);
                    player->setX(player->getX() * player->getGrid());
                    player->setY(player->getY() * player->getGrid());
                break;
                case 8:
                    player->initHoriShooter(config);
                break;
                case 9:
                    player->initRhythm(config);
                break;
                default:
                    break;
            }
        }
        // Stage
        stage->updateScale();
    }
    // Back button
    backButton(input);
}

// void Scene::updateGallery(Input *input)
// {
//     // 6x5 grid, totalling up to 30
//     //  0  1  2  3  4  5
//     //  6  7  8  9 10 11
//     // 12 13 14 15 16 17
//     // 18 19 20 21 22 23
//     // 24 25 26 27 28 29
//     // Final item position tbd
    
//     // Navigation (all 4 directions)
//     if (input->getPress(0) && menu_counter > 5)
//     {
//         menu_counter -= 6;
//     }
//     if (input->getPress(1) && menu_counter < 24)
//     {
//         menu_counter += 6;
//     }
//     if (input->getPress(2) && menu_counter > 0)
//     {
//         menu_counter--;
//     }
//     if (input->getPress(3) && menu_counter < 29)
//     {
//         menu_counter++;
//     }
//     // Choose the highlighted option
//     // Might have a desc zone and hover only instead
//     // No need for clicking
//     if (input->getPress(4))
//     {
//         // Zoom into item, add description
//     }
// }

void Scene::updatePause(Input *input, Stage *stage, Player *player)
{
    // Navigation (top to bottom)
    if (press(Dir::UP, input) && 
        menu_counter > 0)
    {
        menu_counter--;
    }
    if (press(Dir::DOWN, input) && 
        menu_counter < 2)
    {
        menu_counter++;
    }
    // Choose the highlighted option
    if (input->getPress(Action::ACTION1))
    {
        switch (menu_counter)
        {
            case 0: // Resume
                game_state = pre_pause_state;
                menu_counter = 0;
            break;
            case 1: // Settings
                prev_state = game_state;
                game_state = 4;
                menu_counter = 0;
            break;
            case 2: // Quit to menu
                game_state = 1;
                menu_counter = 0;
                // Clear current game session
                stage->unload();
                player->unload();
            break;
            default:
            break;
        }
    }
    // Back button
    backButton(input);
}