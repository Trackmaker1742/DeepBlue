#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

class Input 
{
private:
    // Controller Detection
    SDL_GameController *controller;
    // Input
    std::vector<bool> button;   // 0: up
                                // 1: down
                                // 2: left
                                // 3: right
                                // 4: jump
                                // 5: dash
                                // 6: attack
                                // 7: nothing for now
                                // 8: grid
                                // 9: dev
                                // 10: pause
                                // 11: arrow up
                                // 12: arrow down
                                // 13: arrow left
                                // 14: arrow right

    std::vector<bool> hold;     // Used to handle presses, 
                                // stop hold input from activating in press only scenario (jump, dash,...)
    
public:
    void init();
    bool input();

    // Setters
    void setHold(uint8_t i, bool state);

    // Getters
    bool getPress(uint8_t i);
};

#endif