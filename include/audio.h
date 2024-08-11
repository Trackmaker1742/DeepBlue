#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>
#include <vector>

class Audio
{
private:
    // Audio
    static std::vector<Mix_Music*> bgm;

    static std::vector<Mix_Chunk*> sound_fx;

public:
    // No constructor needed
    void init();

    // Bgm handler
    static void playBgm(int track_number, int loops);
    // SoundFX handler
    // Max channel is 8, so use it with care
    static void playSoundFX(int channel, int track_number, int loops = 0);
    
    ~Audio();
};

#endif