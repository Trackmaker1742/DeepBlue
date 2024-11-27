#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <cmath>
#include <windows.h>
#include <filesystem>
#include <memory>
#include <thread>
#include <atomic>

#include "config.h"
#include "input.h"
#include "renderer.h"
#include "file_handler.h"

#include "object2d.h"
#include "camera.h"
#include "collision.h"
#include "player.h"
#include "block.h"
#include "stage.h"
#include "projectile.h"
#include "enemy.h"
#include "editor.h"
#include "scene.h"

// Multithreading
std::atomic<bool> running(true);
std::thread save_edit_thread;

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    // Play audio
    Audio *audio = new Audio();
    audio->init();
    
    // File testing
    File_Handler *file = new File_Handler();
    file->readConfig();
    file->readSave();

    // Initialize input
    Input *input = new Input();
    input->init();

    // Initialize game config
    Config *config = new Config(input);
    config->init(file->getValue(0), 
        file->getValue(1), 
        file->getValue(2), 
        file->getValue(3));
    
    // Menu
    Scene *scene = new Scene(config->getRenderer());
    scene->initSettingValues(file->getValue(0), 
        file->getValue(1), 
        file->getValue(2), 
        file->getValue(3));
    scene->initMenuTextures(file);

    // Stage
    Stage *stage = new Stage(config->getRenderer());
    stage->init();
    stage->updateScaleFactor(config->getScaleFactor());

    // Camera
    // Only applies to platforming sections as of rn
    Camera *cam = new Camera();

    // Renderer
    Renderer *renderer = new Renderer(config, cam);
    renderer->loadTextureForText(scene);

    // Player
    Player *player = new Player();

    // Collision checker
    Collision *colli = new Collision();

    // Editor mode
    Editor *edit = new Editor();

    // Initialize variable to save the time of the previous frame
    uint64_t prev_frame = SDL_GetTicks64();

    bool quit = false;
    SDL_RaiseWindow(config->getWindow());

    bool first = false;

    while (!input->handleInput() && quit != true)
    {
        // Calculating delta time
        uint64_t current_frame = SDL_GetTicks64();
        config->setDeltaTime((current_frame - prev_frame) / 1000.0f);

        // Display the coresponding game state result
        switch (scene->getState())
        {
            // Quit game
            case 0:
                quit = true;
            break;
            // Main menu
            case 1:
                // if (input->getPress(Action::EXTRA1))
                // {
                //     config->update();
                // }
                scene->updateMain(input);
                renderer->renderMainMenu(scene);
            break;
            // Stage select menu
            case 2:
                scene->updateStageSelect(input, config, file, stage, player, cam);
                renderer->renderStageSelect(scene);
            break;
            // // Gallery
            // case 3:
            //     config->updateGallery();
            //     renderer->renderGallery();
            // break;
            // Settings
            case 4:
                scene->updateSettings(input, config, file, stage, player, cam);
                renderer->renderSettings(scene);
            break;
            // Pause menu
            case 5:
                // Render current game state for cooler looking pause menu
                // Performance intensive but it looks cool
                switch (scene->getPrePauseState())
                {
                    case 6:
                        renderer->renderStagePlat(stage, player, edit); 
                    break;
                    case 7:
                        renderer->renderStageShooter(stage, player);
                        renderer->renderPlayerVertShooter(player);
                    break;
                    case 8:
                        renderer->renderStageShooter(stage, player);
                        renderer->renderPlayerHoriShooter(player);
                    break;
                    case 9:
                        renderer->renderStageRhythm(stage, player);
                    break;
                    default:
                    break;
                }
                scene->updatePause(input, stage, player);
                renderer->renderPauseMenu(scene);
            break;
            // Platforming stage + level mod (won't be in final version)
            case 6:
                // Audio test (works)
                // input 7 is u key
                // if (input->getPress(7))
                // {
                //     input->setHold(7, false);
                //     Audio::playSoundFX(1, 0);
                // }
                // if (input->getPress(8))
                // {
                //     input->setHold(8, false);
                //     Audio::playSoundFX(2, 0);
                // }
                // Input mapping (works)
                // if (input->getPress(Action::MOVE_UP))
                // {
                //     input->setHold(Action::MOVE_UP, false);
                //     input->waitForKeyRemap(Action::ACTION1);
                // }

                // Platforming stages
                if (!player->getEditor())
                {
                    // Pause menu
                    scene->pauseHandler(input);
                    // Player movement
                    player->platformerMvt(input, config->getDeltaTime());
                    // Block update
                    stage->update(config->getDeltaTime());
                    // Collision handler
                    colli->playerBlockColli(stage, player, config->getDeltaTime());
                }
                else
                {
                    player->editorMvt(input, config->getDeltaTime());
                    edit->menuAction(input, player, stage);
                    if (edit->getSaving())
                    {
                        running = true;
                        // Setting max size for the layers
                        edit->init();
                        for (int i = 0; i < 4; i++) edit->setMaxXYSize(stage->getBlockVec(i));
                        
                        if (edit->getChanged(0))
                        {
                            save_edit_thread = std::thread(Editor::saveMovingBlocks, edit, 
                                stage->getBlockVec(0),
                                stage->getLayerDir(0));
                        }
                        if (edit->getChanged(1))
                        {
                            save_edit_thread = std::thread(Editor::saveNormalBlocks, edit, 
                                stage->getBlockVec(1),
                                stage->getLayerDir(1));
                        }
                        if (edit->getChanged(2))
                        {
                            save_edit_thread = std::thread(Editor::saveNoColliBlocks, edit, 
                                stage->getBlockVec(2),
                                stage->getLayerDir(2), 2);
                        }
                        if (edit->getChanged(3))
                        {
                            save_edit_thread = std::thread(Editor::saveNoColliBlocks, edit, 
                                stage->getBlockVec(3),
                                stage->getLayerDir(3), 3);
                        }
                        edit->completeSaving();
                        save_edit_thread.detach();
                    }
                }
                // Camera update
                cam->updatePlatCam(player, config->getDeltaTime());
                // Render stuff
                renderer->renderStagePlat(stage, player, edit);
            break;

            // WIP
            // Vertical shooting stage
            case 7:
                // Pause menu
                scene->pauseHandler(input);

                player->shooterMvt(input, config->getDeltaTime(), config->getWidth(), config->getHeight());
                renderer->renderStageShooter(stage, player);
                renderer->renderPlayerVertShooter(player);
            break;
            // Horizontal shooting stage
            case 8:
                // Pause menu
                scene->pauseHandler(input);

                player->shooterMvt(input, config->getDeltaTime(), config->getWidth(), config->getHeight());
                player->shooterHoriAtk(config, input, config->getDeltaTime());
                renderer->renderStageShooter(stage, player);
                renderer->renderPlayerHoriShooter(player);
            break;
            // Rhythm stage
            case 9:
                // Pause menu
                scene->pauseHandler(input);

                player->rhythmMvt(input, config->getDeltaTime());
                cam->updateRhyCam(player, config->getDeltaTime());
                renderer->renderStageRhythm(stage, player);
            break;
            case 10:
            break;
            default:
            break;
        }
        
        // To make press input work
        input->unHold();
        // Present renderer
        SDL_RenderPresent(config->getRenderer());
        // Get prev frame time (for delta_time)
        prev_frame = current_frame;
        // Framerate handler (cap to FPS)
	    SDL_Delay(1000.0f/config->getFPS() - config->getDeltaTime());
    }

    running = false;
    if (save_edit_thread.joinable()) save_edit_thread.join();

    delete audio;
    delete input;
    delete stage;
    delete cam;
    delete renderer;
    delete player;

    SDL_DestroyRenderer(config->getRenderer());
    SDL_DestroyWindow(config->getWindow());
    delete config;
    
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

// Currently unused, dunno about the future
// Will be used in level editor mode
// Zoom (working perfectly)
// if (input->getPress(11) || input->getPress(12))
// {
//     float temp = 0;
//     // Zoom closer
//     if (input->getPress(11))
//     {
//         input->setHold(11, false);
//         temp = 4;
//     }
//     // Zoom farther
//     if (input->getPress(12))
//     {
//         input->setHold(12, false);
//         temp = -4;
//     }
//     player->setX(player->getX() + temp * (player->getX() / float(player->getGrid())));
//     player->setY(player->getY() + temp * (player->getY() / float(player->getGrid())));
//     player->setGrid(player->getGrid() + temp);
//     for (Block *b : stage->getNormalBVec())
//     {
//         b->setGrid(b->getGrid() + temp);
//     }
// }