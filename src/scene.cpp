#include "scene.h"

Scene::Scene(SDL_Renderer *rend) : renderer(rend)
{ }

// Getters
SDL_Texture *Scene::getBgTexture(MenuIndex m_index, int index) { return bg_texture_matrix[int(m_index)][index]; }
uint8_t Scene::getState() { return game_state; }
uint8_t Scene::getPrevState() { return prev_state; }
uint8_t Scene::getCounter() { return menu_counter; }
uint8_t Scene::getStageNum() { return stage_number; }

// Setters
void Scene::setState(uint8_t st) { game_state = st; }

void Scene::backButton(Input *input)
{
    // Back button
    if (input->getPress(Action::ACTION2))
    {
        input->setHold(Action::ACTION2, false);
        game_state = prev_state; // Go back to previous state
        menu_counter = 0;
    }
}

// Scene stuff
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
    File_Handler *file, Stage *stage, Player *player)
{
    // Change game state (manual for now)
    switch (stage_number)
    {
        // Platforming init
        case 1: case 2: case 4: case 6: case 8:
            stage->initPlatAll(file, stage_number + 48);
            player->initPlat(config->getRenderer());
            // Set spawn
            player->setX(stage->getRespX());
            player->setY(stage->getRespY());
            game_state = 6;
        break;
        // Vertical shooter init
        case 3:
            stage->initVertShooterAll(file, stage_number + 48);
            player->initVertShooter(config->getRenderer());
            // Set spawn
            player->setX(config->getWidth() / 2);
            player->setY(config->getHeight() / 2);
            game_state = 7;
        break;
        // Horizontal shooter init
        case 5:
            stage->initHoriShooterAll(file, stage_number + 48);
            player->initHoriShooter(config->getRenderer());
            // Set spawn
            player->setX(config->getWidth() / 2);
            player->setY(config->getHeight() / 2);
            game_state = 8;
        break;
        // Rhythm init
        case 7:
            // Stage init rhythm all
            player->initRhythm(config->getRenderer());
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
        input->setHold(Action::PAUSE, false);
        prev_state = game_state;
        game_state = 5;
        menu_counter = 0;
    }
}

// Update Scene (mainly input, render in renderer please!)
void Scene::updateMain(Input *input)
{
    // Navigation (top to bottom)
    if (input->getPress(Action::MOVE_UP) && menu_counter > 0)
    {
        input->setHold(Action::MOVE_UP, false);
        menu_counter--;
    }
    if (input->getPress(Action::MOVE_DOWN) && menu_counter < 4)
    {
        input->setHold(Action::MOVE_DOWN, false);
        menu_counter++;
    }
    // Choose the highlighted option
    if (input->getPress(Action::ACTION1))
    {
        input->setHold(Action::ACTION1, false);
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
    File_Handler *file, Stage *stage, Player *player)
{
    // Will need more condition, 
    // since you can only pick stages after unlocking them
    // Navigation (left to right)
    if (input->getPress(Action::MOVE_LEFT) && menu_counter > 0)
    {
        input->setHold(Action::MOVE_LEFT, false);
        menu_counter--;
    }
    if (input->getPress(Action::MOVE_RIGHT) && menu_counter < 8)
    {
        input->setHold(Action::MOVE_RIGHT, false);
        menu_counter++;
    }
    // Choose the highlighted option
    if (input->getPress(Action::ACTION1))
    {
        input->setHold(Action::ACTION1, false);
        stage_number = menu_counter + 1;    // For loading the correct stage
        initStage(config, file, stage, player);
        menu_counter = 0;
    }
    // Back button
    backButton(input);
}

void Scene::updateSettings(Input *input)
{
    // Navigation (top to bottom)
    if (input->getPress(Action::MOVE_UP) && menu_counter > 0 && menu_counter != 4)
    {
        input->setHold(Action::MOVE_UP, false);
        menu_counter--;
    }
    if (input->getPress(Action::MOVE_DOWN) && menu_counter < 3)
    {
        input->setHold(Action::MOVE_DOWN, false);
        menu_counter++;
    }
    // Side to side for save and back button
    if (input->getPress(Action::MOVE_LEFT) && menu_counter == 3)
    {
        input->setHold(Action::MOVE_LEFT, false);
        menu_counter++;
    }
    if (input->getPress(Action::MOVE_RIGHT) && menu_counter == 4)
    {
        input->setHold(Action::MOVE_RIGHT, false);
        menu_counter--;
    }
    // Press save
    if (input->getPress(Action::ACTION1) && menu_counter == 3)
    {
        input->setHold(Action::ACTION1, false);
        switch (menu_counter)
        {
            case 3:
                // Save the new config
            break;
            case 4:
                game_state = prev_state; // Go back to previous state
                menu_counter = 0;
            break;
            default:
            break;
        }
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
//         input->setHold(0, false);
//         menu_counter -= 6;
//     }
//     if (input->getPress(1) && menu_counter < 24)
//     {
//         input->setHold(1, false);
//         menu_counter += 6;
//     }
//     if (input->getPress(2) && menu_counter > 0)
//     {
//         input->setHold(2, false);
//         menu_counter--;
//     }
//     if (input->getPress(3) && menu_counter < 29)
//     {
//         input->setHold(3, false);
//         menu_counter++;
//     }
//     // Choose the highlighted option
//     // Might have a desc zone and hover only instead
//     // No need for clicking
//     if (input->getPress(4))
//     {
//         input->setHold(4, false);
//         // Zoom into item, add description
//     }
// }

void Scene::updatePause(Input *input, Stage *stage, Player *player)
{
    // Navigation (top to bottom)
    if (input->getPress(Action::MOVE_UP) && menu_counter > 0)
    {
        input->setHold(Action::MOVE_UP, false);
        menu_counter--;
    }
    if (input->getPress(Action::MOVE_DOWN) && menu_counter < 2)
    {
        input->setHold(Action::MOVE_DOWN, false);
        menu_counter++;
    }
    // Choose the highlighted option
    if (input->getPress(Action::ACTION1))
    {
        input->setHold(Action::ACTION1, false);
        switch (menu_counter)
        {
            case 0: // Resume
                game_state = prev_state;
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