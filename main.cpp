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
#include "projectile.h"
#include "enemy.h"

int main(int argc, char *argv[])
{
    // // Play audio
    // Mix_Music *music = Mix_LoadMUS("res/Audio/BGM/KoiIsLoveLofi.wav");
    // // music = Mix_LoadMUS("res/Audio/BGM/StringTheory.wav");
    // Mix_PlayMusic(music, -1);
    
    // File testing
    File_Handler *test = new File_Handler();
    test->readConfig();
    test->readSave();

    // Initialize input
    Input *input = new Input();
    input->init();

    // Initialize game scene
    Scene *scene = new Scene(input);
    scene->init(test->getValue(0), test->getValue(1), test->getValue(2), test->getValue(3));
    scene->initAllMenu();
    scene->setState(6);

    // Proj test
    Projectile *proj = new Projectile(10, 10, "");

    // Camera
    // Only applies to platforming sections as of rn
    Camera *cam = new Camera();
    cam->init(scene);

    // Renderer
    Renderer *renderer = new Renderer(scene, cam);

    // Stage 
    Stage *stage = new Stage(scene->getRenderer());
    stage->initPlatAll("data/stage1.csv");

    // Player
    Player *player = new Player(scene->getFPS(), stage->getRespX(), stage->getRespY(), "res/Player Sprites/pixel.png");
    player->init(scene->getRenderer());

    // Collision checker
    Collision *colli = new Collision();

    // Initialize variable to save the time of the previous frame
    uint64_t prev_frame = SDL_GetTicks64();

    bool quit = false;

    while (!input->input() && quit != true)
    {
        // Calculating delta time
        uint64_t current_frame = SDL_GetTicks64();
        scene->setDeltaTime((current_frame - prev_frame) / 1000.0f);

        // Display the coresponding game state result
        switch (scene->getState())
        {
            // Quit game
            case 0:
                quit = true;
            break;
            // Main menu
            case 1:
                // Template for all menu stuff
                scene->updateMainMenu();
                renderer->renderMainMenu();
            break;
            // Stage select menu
            case 2:
                scene->updateStageSelect();
                renderer->renderStageSelect();
            break;
            // Gallery
            case 3:
                scene->updateGallery();
                renderer->renderGallery();
            break;
            // Settings
            case 4:
                scene->updateSettings();
                renderer->renderSettings();
            break;
            // Pause menu
            case 5:
                scene->updatePauseMenu();
                renderer->renderPauseMenu();
            break;
            // Platforming stage
            case 6:
                // Platforming only, will try switch case to work out the states later
                // Player movement
                player->playerPlatMvt(input, scene->getDeltaTime());
                // Collision handler
                colli->playerBlockColli(stage, player, stage->getBlockVec());
                colli->playerPrev(player);
                // Camera update
                cam->updatePlatCam(player);
                // Render stuff
                renderer->renderStage(stage, player);
            break;

            // WIP
            // Vertical shooting stage
            case 7:

            break;
            // Horizontal shooting stage
            case 8:

            break;
            // Rhythm stage
            case 9:
                player->playerRhythmMvt(input, scene->getDeltaTime());
                renderer->renderStage(stage, player);
            break;
            // Cutscenes
            case 10:

            break;

            default:
            break;
        }
        
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

// Currently unused, dunno about the future
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
//     for (Block *b : stage->getBlockVec())
//     {
//         b->setGrid(b->getGrid() + temp);
//     }
// }

// 2 extra game mode, menu, assets, will I make it in time?
// Today is August 3rd, I have less than 2 weeks left until the 15th...
// Moving background, item collision, block re-ordering
// Making it so that new stages can be added more easily
// Create a template for all the needed stage assets
// Such as leaving 10 to 20 (example) for normal block,...
// Moving block needs to be worked on as well
// All the non-physical blocks will be moved up, the rest will be moved down
// I made alright progress today, however, I was able to stay focus for way longer than usual
// Thanks, Ayase-san

// Alright, reordering the blocks come first
// I will move onto game state, menu and all that now
// Save file is done? Kinda, stage progress and item should be ok now
// I still have to write to those files tho
// Both config and save need a write function
// A little frustrated that my friend dropped out, I was so eager to see his work
// Someone else will have to do, I guess
// I wonder if the new guy can make it
// Time is of the essence, and I probably won't be able to hire another guy after this, 
// with how little time I have left
// Camera isn't needed for the other 3 game mode
// Enemy pathing and attacks...
// Sounds rough
// The rhythm game mode should be easier though
// With the player standing still (x-axis) and obstacles coming their way
// The player can move around in the other 2 mode, but there will be boundaries
// No need to use camera to follow their movements
// It will be like Touhou and Cuphead fly section, respectively
// No need to go too crazy on the sprites for the Touhou part (Just a plain flycar of sorts)
// But the Cuphead section (Gemini Syndrome) and rhythm (Endorfin.) do

// Today is August 5th, let's continue making the game, shall we
// I'll start with the menu first
// The plan right now is to pump the game out as early as possible without sacrificing the quality
// Therefore, I'll have to forfeit the plan of condensing each stage into their own folder for now
// Assets will be shared, there won't be any dynamic level adding or anything like that
// I should get to moving background next, I'll list the gist of my idea down here
// Platforming: player speed dependent
// 3 other game mode: move based on a counter that increments then loop back
// Surprisingly easier than I thought
// Minor correction: moving background should be based on x and y positions instead of velocity
// Now we loop the background (done)
// Off to menu work, remember to render in renderer
// Why? moving background, long,...
// Probably time to touch on the other game mode
// Rhythm first?
// Side note, I'm hungry for once
// Been a few days since I've last felt this
// Class, even when you don't study can get exhausting, huh
// I think I'll have the player move on rail, actually
// Having all the blocks move sounds like a pain
// Gonna have to rename a lot of stuff after this
// The barebone for rhythm mode is done now
// Time to do the shooter mode

// Vertical shooting mode:
// Start with slow projectile (level 0)
// Collect first item (Nakuru's hairpin) to upgrade to the next level
// Taking damage will decrease a level, taking damage at level 0 will force a restart
// Level 1: Fast blue projectile (1 dmg)
// Level 2: Fast blue + yellow projectile (2 dmg per tick)
// Level 3: Fast blue + yellow + red projectile (3 dmg per tick)
// Level 4: Laser beam (max level, 4 dmg per tick)

// Horizontal shooting mode:
// Ikaruga style cuz I'm out of idea
// No item, but better moveset
// Shoot at enemy to fill up a meter
// Use meter for Itsuki's yellow laser beam, can interrupt certain boss moves
// Laser beam pushes you back a bit
// Shoot dark bullets with Nakuru, invul to dark bullets from enemy
// Shoot light bullets with Nayuta, invul to light bullet from enemy

// Welp, better learn how to use destructor now
// Note: deleting the stuff from sdl can get weird
// And only delete the stuff from the object that OWNS the pointer
// I'll have to create enemies and a boss...
// Took forever to deal with the sudden compiler issue
// Dang it