#include "audio.h"

std::vector<Mix_Music*> Audio::bgm;
std::vector<Mix_Chunk*> Audio::sound_fx;

// Initialize
void Audio::init() 
{
    // Set up the audio stream
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);

    // Load bgm waveforms
    std::vector<const char*> bgm_files =
    {
        ""
    };
    for (const char *file : bgm_files)
    {
        bgm.push_back(Mix_LoadMUS(file));
    }
    // bgm_files.clear();

    // Load sound_fx waveforms
    std::vector<const char*> sound_fx_files =
    {
        "res/Audio/SFX/Snare-Drum-1.wav",
        "res/Audio/SFX/Kick-Drum-1.wav"
    };
    for (const char *file : sound_fx_files)
    {
        sound_fx.push_back(Mix_LoadWAV(file));
    }
    // sound_fx_files.clear();
}

void Audio::playBgm(int track_number, int loops)
{
    Mix_PlayMusic(bgm[track_number], loops);
}

void Audio::playSoundFX(int channel, int track_number, int loops)
{
    Mix_PlayChannel(channel, sound_fx[track_number], loops);
}

Audio::~Audio()
{
    // Unloading audio
    for (Mix_Music *b : bgm)
    {
        Mix_FreeMusic(b);
    }
    for(Mix_Chunk *sfx : sound_fx)
    {
        Mix_FreeChunk(sfx);
    }
    Mix_CloseAudio();
}