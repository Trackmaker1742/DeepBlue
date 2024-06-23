#include "stage.h"

Stage::Stage(SDL_Renderer *rend) : renderer(rend)
{ }

// Getters
SDL_Texture *Stage::getBackground()
{
    return bg;
}
std::vector<Block*> Stage::getBlockVec()
{
    return Blocks;
}

// Initialize
void Stage::initAudio() 
{
    const char* _waveFileNames[] =
    {
        "res/Audio/SFX/Snare-Drum-1.wav",
        "res/Audio/SFX/Kick-Drum-1.wav",
        "res/Audio/SFX/CombatReady.wav",
        "res/Audio/SFX/Hurt.wav",
        "res/Audio/SFX/NakuHurt.wav",
        "res/Audio/SFX/Parry.wav",
    };
    memset(_sample, 0, sizeof(Mix_Chunk*) * 2);

    // Set up the audio stream
    int result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);

    result = Mix_AllocateChannels(4);

    // Load waveforms
    for( int i = 0; i < 6; i++ )
    {
        _sample[i] = Mix_LoadWAV(_waveFileNames[i]);
    }
}

void Stage::initBackground()
{
    SDL_Surface* surface = IMG_Load("res/Serene Blue.png");
    bg = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Stage::initBlocks(const char *path)
{
    // Read stage layout straight from csv file
    File_Handler *file = new File_Handler();

    file->readCSV(path);

    for (int i = 0; i < file->getStageInt().size(); i++)
    {
        for (int j = 0; j < file->getStageInt()[i].size(); j++)
        {
            // Add blocks, skip 0 (empty space)
            // Numeric data for block (coming soon)
            if (file->getStageInt()[file->getStageInt().size() - 1 - i][j] != 0) Blocks.push_back(new Block(j, i, "res/Test.png"));
        }
    }

    delete file;

    for (Block *b : Blocks)
    {
        b->initTexture(renderer);
    }
}

void Stage::initAll(const char *path)
{
    initAudio();
    initBackground();
    initBlocks(path);
}

void Stage::unloadAudio()
{
    // Unloading audio
    for(int i = 0; i < 6; i++)
    {
        Mix_FreeChunk(_sample[i]);
    }
    Mix_CloseAudio();
}