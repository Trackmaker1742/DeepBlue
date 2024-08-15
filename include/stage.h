#ifndef STAGE_H
#define STAGE_H

#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string.h>

#include "file_handler.h"
#include "block.h"
#include "enemy.h"
#include "projectile.h"

class Stage
{
private:
    SDL_Renderer *renderer; // Not belonging to Stage

    // Background textures
    std::vector<SDL_Texture*> background_layers;
    
    // Back and normal block layout in integer array
    std::vector<std::vector<uint8_t>> block_int;
    // Foreground element layout in integer array
    std::vector<std::vector<uint8_t>> front_int;

    // Back and normal block array
    std::vector<Block*> blocks;
    // Front layer array
    std::vector<Block*> front_blocks;

    // Projectile array
    std::vector<Projectile*> projectiles;

    // Respawn coordinates
    uint16_t resp_x;
    uint16_t resp_y;

    // Background paths
    std::vector<std::string> background_paths;

    // Block sprite path (able to detect the block type in csv)
    std::vector<std::string> block_paths;
        // "res/BlockSprite/Spawn.png",
        // "res/BlockSprite/Checkpoint.png",
        // "res/BlockSprite/Goal.png",
        // "res/BlockSprite/Danger.png",
        // "res/BlockSprite/Item.png",
        // "res/BlockSprite/Normal.png",
        // "res/BlockSprite/Slope (Left).png",
        // "res/BlockSprite/Slope (Right).png",
        // "res/BlockSprite/Wall.png",
        // "res/BlockSprite/Normal.png"

    // Rhythm obstacles sprites
    // High geyser sprite will only contain the bottom half,
    // with the top half using the same sprite as low geyser
    std::vector<std::string> obstacle_path;
        // "res/BlockSprite/Spawn.png",
        // "res/BlockSprite/Normal.png",
        // "res/BlockSprite/Danger.png",
        // "res/BlockSprite/Wall.png",
            
public:
    Stage(SDL_Renderer *rend);

    // Getters
    std::vector<SDL_Texture*> getBackgroundLayers();
    std::vector<Block*> getBlockVec();
    std::vector<Projectile*> getProjVec();
    uint16_t getRespX();
    uint16_t getRespY();

    // Setters
    void setRespX(uint16_t x);
    void setRespY(uint16_t y);

    // Initialize
    void initSpritePath(char stage_number);
    void initBackground();
    // Background blocks and normal blocks can be in the same csv
    // So splitting them might not be needed
    void initBlockLayer(char stage_number);  // Block layer (interactable)
    void initFrontLayer(char stage_number);  // Foreground elements (grass, stuff rendered after the player)
    void initPlatAll(char stage_number);

    void initRhyObs(char stage_number);
    void initRhyAll(char stage_number);

    // Unload everything in a stage (blocks, spawn)
    void unloadStage();

    ~Stage();
};

#endif