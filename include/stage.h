#ifndef STAGE_H
#define STAGE_H

#include <SDL2/SDL_mixer.h>
#include <vector>

#include "file_handler.h"
#include "block.h"
#include "enemy.h"
#include "projectile.h"

class Stage
{
private:
    SDL_Renderer *renderer; // Not belonging to Stage
    
    // Background
    SDL_Texture *bg;
    
    // Audio
    Mix_Chunk *_sample[6];

    // Block array
    std::vector<Block*> Blocks;

    // Respawn coordinates
    uint16_t resp_x;
    uint16_t resp_y;

    // Block sprite path (able to detect the block type in csv)
    // Now go get some block sprites :33
    std::vector<const char*> block_path
    {
        "",
        "res/BlockSprite/Spawn.png",
        "res/BlockSprite/Checkpoint.png",
        "res/BlockSprite/Goal.png",
        "res/BlockSprite/Danger.png",
        "res/BlockSprite/Item.png",
        "res/BlockSprite/Normal.png",
        "res/BlockSprite/Slope (Left).png",
        "res/BlockSprite/Slope (Right).png",
        "res/BlockSprite/Wall.png",
        "res/BlockSprite/Normal.png"
    };

    // Rhythm obstacles sprites
    // High geyser sprite will only contain the bottom half,
    // with the top half using the same sprite as low geyser
    std::vector<const char*> obstacle_path
    {
        "",
        "res/BlockSprite/Spawn.png",
        "res/BlockSprite/Normal.png",
        "res/BlockSprite/Danger.png",
        "res/BlockSprite/Wall.png",
    };
    
public:
    Stage(SDL_Renderer *rend);

    // Getters
    SDL_Texture *getBackground();
    std::vector<Block*> getBlockVec();
    uint16_t getRespX();
    uint16_t getRespY();

    // Setters
    void setRespX(uint16_t x);
    void setRespY(uint16_t y);

    // Initialize
    void initAudio();
    void initBackground();
    void initBlocks(const char *path);
    void initPlatAll(const char *path);

    void initRhyObs(const char *path);
    void initRhyAll(const char *path);

    // Unload everything in a stage (blocks, spawn)
    void unloadStage();

    // Will be combined with unloadStage once I get to audio
    void unloadAudio();

    ~Stage();
};

#endif