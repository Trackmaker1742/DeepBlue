#ifndef STAGE_H
#define STAGE_H

#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string.h>
#include <sstream>
#include <algorithm>

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
    
    // Moving block and normal block has to be in the same array
    // to assign x and y coords
    std::vector<std::vector<std::string>> block_str;
    // Foreground element layout in integer array
    std::vector<std::vector<uint8_t>> front_str;

    // Back and normal block array
    std::vector<Block*> blocks;
    // Moving block arrays (for easier parsing)
    // Things to note with moving block, it's annoying to have the block
    // move left and down due to negative number string
    // So when making moving block, it's recommended to start 
    // from bottom left of the movement pattern
    std::vector<Block*> moving_blocks;    // Initial position of moving blocks
    // Front layer array
    std::vector<Block*> front_blocks;

    // Projectile array
    std::vector<Projectile*> projectiles;

    // Respawn coordinates
    uint16_t resp_x;
    uint16_t resp_y;

    // Background paths
    std::vector<std::string> background_paths;
    bool background_move = false;

    // Block sprite path (able to detect the block type in csv)
    std::vector<std::string> block_paths;

    // Rhythm obstacles sprites
    // High geyser sprite will only contain the bottom half,
    // with the top half using the same sprite as low geyser
    std::vector<std::string> obstacle_path;
            
public:
    Stage(SDL_Renderer *rend);

    // Getters
    std::vector<SDL_Texture*> getBackgroundLayers();
    std::vector<Block*> getBlockVec();
    std::vector<Block*> getMovingBlockVec();
    std::vector<Projectile*> getProjVec();
    uint16_t getRespX();
    uint16_t getRespY();
    bool getBgMove();

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

    // Update for moving blocks
    void update(float dt);

    // Unload everything in a stage (blocks, spawn)
    void unloadStage();

    ~Stage();
};

#endif