#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <unordered_map>
#include <functional>
#include <vector>
#include <iostream>
#include <algorithm>

enum class Action 
{
    // Base player inputs
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ACTION1,
    ACTION2,
    ACTION3,

    // Extra inputs for debug mode
    EXTRA_UP,
    EXTRA_DOWN,
    EXTRA_LEFT,
    EXTRA_RIGHT,
    EXTRA1,
    EXTRA2,
    EXTRA3,
    EXTRA4,

    // Pause menu
    PAUSE,

    ACTION_MAX  // Total number of actions
};

class Input 
{
private:
    std::unordered_map<SDL_Keycode, Action> key_to_action;
    std::unordered_map<SDL_GameControllerButton, Action> controller_button_to_action;

    std::vector<bool> button;
    std::vector<bool> hold;
    std::vector<bool> press;

    void handleKeyboardEvent(const SDL_KeyboardEvent &keyEvent);
    // void handleControllerButtonEvent(const SDL_ControllerButtonEvent &buttonEvent);

public:
    void init();
    bool handleInput();

    // void setHold(Action action, bool hol);
    bool getPress(Action action);
    bool getHold(Action action);

    // Cleaner way to deal with presses
    // Instead of using setHold for every single time a press is needed
    // There will be a getHold and getPress, and every loop will unHold every input
    // getHold won't be affected, as it will only rely on button value being returned
    void unHold();

    void remapKey(SDL_Keycode key, Action action);
    void remapControllerButton(SDL_GameControllerButton button, Action action);
    void waitForKeyRemap(Action action_to_remap);
};

#endif