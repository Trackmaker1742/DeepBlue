#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <cmath>
#include <Windows.h>

#include "scene.h"
#include "input.h"
#include "renderer.h"
#include "file_handler.h"

#include "object2d.h"
#include "camera.h"
#include "collision.h"
#include "player.h"
#include "block.h"
#include "stage.h"

int main(int argc, char *argv[])
{
    // // Play audio
    // Mix_Music *music = Mix_LoadMUS("res/Audio/BGM/KoiIsLoveLofi.wav");
    // // music = Mix_LoadMUS("res/Audio/BGM/StringTheory.wav");
    // Mix_PlayMusic(music, -1);
    
    // File testing
    File_Handler *test = new File_Handler();
    test->readConfig(); 
    test->readCSV("data/stage_layout.csv");

    // Initialize game scene
    Scene *scene = new Scene();
    scene->init(test->getValue(0), test->getValue(1), test->getValue(2), test->getValue(3));

    // Initialize input
    Input *input = new Input();
    input->init();

    // Camera
    Camera *cam = new Camera();
    cam->init(scene);

    // Renderer
    Renderer *renderer = new Renderer(scene, cam);

    // Player
    Player *player = new Player(200, 100, "res/Drool.png");
    player->init(scene->getRenderer());

    // Stage 
    Stage *stage = new Stage(scene->getRenderer());
    stage->initAll();

    // Collision checker
    Collision *colli = new Collision();

    // Initialize variable to save the time of the previous frame
    uint64_t prev_frame = SDL_GetTicks64();

    while (!input->input())
    {
        // Calculating delta time
        uint64_t current_frame = SDL_GetTicks64();
        scene->setDeltaTime((current_frame - prev_frame) / 1000.0f);
        // std::cout << scene->getDeltaTime() << "\n";

        // Player movement
        player->playerMovement(input, scene->getDeltaTime());

        // Collision handler
        colli->playerBlockColli(player, stage->getBlockVec());
        colli->playerPrev(player);

        // Camera update
        cam->updateCam(player);

        // Zoom test
        if (input->getPress(11))
        {
            input->setHold(11, false);
            player->setGrid(player->getGrid() + 4);
            for (Block *b : stage->getBlockVec())
            {
                b->setGrid(b->getGrid() + 4);
            }
        }
        if (input->getPress(12))
        {
            input->setHold(12, false);
            player->setGrid(player->getGrid() - 4);
            for (Block* b : stage->getBlockVec())
            {
                b->setGrid(b->getGrid() - 4);
            }
        }

        // Render stuff
        renderer->renderStage(stage, player);
        // std::cout << player->getX() << " " << player->getY() << "\n";

        // Present renderer
        SDL_RenderPresent(scene->getRenderer());

        // Get prev frame time (for delta_time)
        prev_frame = current_frame;
        // Framerate handler (cap to FPS)
	    SDL_Delay(1000.0f/scene->getFPS() - scene->getDeltaTime());
    }

    SDL_DestroyRenderer(scene->getRenderer());
    SDL_DestroyWindow(scene->getWindow());
    SDL_Quit();

    return EXIT_SUCCESS;
}