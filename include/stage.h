#ifndef STAGE_H
#define STAGE_H

#include <SDL2/SDL_mixer.h>
#include <vector>

#include "file_handler.h"
#include "block.h"

class Stage
{
private:
    SDL_Renderer *renderer;
    
    // Background
    SDL_Texture *bg;
    
    // Audio
    Mix_Chunk *_sample[6];

    // Block array
    std::vector<Block*> Blocks;
public:
    Stage(SDL_Renderer *rend);

    // Getters
    SDL_Texture *getBackground();
    std::vector<Block*> getBlockVec();

    // Initialize
    void initAudio();
    void initBackground();
    void initBlocks(const char *path);
    void initAll(const char *path);

    void unloadAudio();
};

#endif