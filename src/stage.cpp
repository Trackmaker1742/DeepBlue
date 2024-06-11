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

void Stage::initBlocks()
{
    // Left boundary
    Blocks.push_back(new Block(0, 1, "res/Test.png"));
    Blocks.push_back(new Block(0, 2, "res/Test.png"));
    Blocks.push_back(new Block(0, 3, "res/Test.png"));
    Blocks.push_back(new Block(0, 4, "res/Test.png"));
    Blocks.push_back(new Block(0, 5, "res/Test.png"));
    Blocks.push_back(new Block(0, 6, "res/Test.png"));
    Blocks.push_back(new Block(0, 7, "res/Test.png"));
    Blocks.push_back(new Block(0, 8, "res/Test.png"));
    Blocks.push_back(new Block(0, 9, "res/Test.png"));

    // Ground
    Blocks.push_back(new Block(0, 0, "res/Test.png"));
    Blocks.push_back(new Block(1, 0, "res/Test.png"));
    Blocks.push_back(new Block(2, 0, "res/Test.png"));
    Blocks.push_back(new Block(3, 0, "res/Test.png"));
    Blocks.push_back(new Block(4, 0, "res/Test.png"));
    Blocks.push_back(new Block(5, 0, "res/Test.png"));
    Blocks.push_back(new Block(6, 0, "res/Test.png"));
    Blocks.push_back(new Block(7, 0, "res/Test.png"));
    Blocks.push_back(new Block(8, 0, "res/Test.png"));
    Blocks.push_back(new Block(9, 0, "res/Test.png"));
    Blocks.push_back(new Block(10, 0, "res/Test.png"));
    Blocks.push_back(new Block(11, 0, "res/Test.png"));
    Blocks.push_back(new Block(12, 0, "res/Test.png"));
    Blocks.push_back(new Block(13, 0, "res/Test.png"));
    Blocks.push_back(new Block(14, 0, "res/Test.png"));
    Blocks.push_back(new Block(15, 0, "res/Test.png"));
    Blocks.push_back(new Block(16, 0, "res/Test.png"));
    Blocks.push_back(new Block(17, 0, "res/Test.png"));
    Blocks.push_back(new Block(18, 0, "res/Test.png"));
    Blocks.push_back(new Block(19, 0, "res/Test.png"));
    Blocks.push_back(new Block(20, 0, "res/Test.png"));
    Blocks.push_back(new Block(21, 0, "res/Test.png"));
    Blocks.push_back(new Block(22, 0, "res/Test.png"));
    Blocks.push_back(new Block(23, 0, "res/Test.png"));
    Blocks.push_back(new Block(24, 0, "res/Test.png"));
    Blocks.push_back(new Block(25, 0, "res/Test.png"));
    Blocks.push_back(new Block(26, 0, "res/Test.png"));
    Blocks.push_back(new Block(27, 0, "res/Test.png"));
    Blocks.push_back(new Block(28, 0, "res/Test.png"));
    Blocks.push_back(new Block(29, 0, "res/Test.png"));
    Blocks.push_back(new Block(30, 0, "res/Test.png"));
    Blocks.push_back(new Block(31, 0, "res/Test.png"));
    Blocks.push_back(new Block(32, 0, "res/Test.png"));
    Blocks.push_back(new Block(33, 0, "res/Test.png"));
    Blocks.push_back(new Block(34, 0, "res/Test.png"));
    Blocks.push_back(new Block(35, 0, "res/Test.png"));
    Blocks.push_back(new Block(36, 0, "res/Test.png"));
    Blocks.push_back(new Block(37, 0, "res/Test.png"));
    Blocks.push_back(new Block(38, 0, "res/Test.png"));
    Blocks.push_back(new Block(39, 0, "res/Test.png"));
    Blocks.push_back(new Block(40, 0, "res/Test.png"));

    // Pillar 1
    Blocks.push_back(new Block(5, 1, "res/Test.png"));
    Blocks.push_back(new Block(5, 2, "res/Test.png"));
    Blocks.push_back(new Block(5, 3, "res/Test.png"));

    // Pillar 2
    Blocks.push_back(new Block(6, 1, "res/Test.png"));
    Blocks.push_back(new Block(6, 2, "res/Test.png"));
    Blocks.push_back(new Block(6, 3, "res/Test.png"));
    Blocks.push_back(new Block(6, 4, "res/Test.png"));

    for (Block *b : Blocks)
    {
        b->initTexture(renderer);
    }
}

void Stage::initStage()
{
    initAudio();
    initBackground();
    initBlocks();
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