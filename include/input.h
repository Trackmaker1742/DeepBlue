#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <unordered_map>
#include <functional>
#include <vector>
#include <iostream>

enum class Action 
{
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    ACTION1,
    ACTION2,
    ACTION_MAX  // Total number of actions
};

class Input 
{
private:
    std::unordered_map<SDL_Keycode, Action> key_to_action;
    std::unordered_map<SDL_GameControllerButton, Action> controller_button_to_action;

    std::vector<bool> button;
    std::vector<bool> hold;

    void handleKeyboardEvent(const SDL_KeyboardEvent &keyEvent);
    // void handleControllerButtonEvent(const SDL_ControllerButtonEvent &buttonEvent);

public:
    void init();
    bool handleInput();

    void setHold(Action action, bool hol);
    bool getPress(Action action);

    void remapKey(SDL_Keycode key, Action action);
    void remapControllerButton(SDL_GameControllerButton button, Action action);
    void waitForKeyRemap(Action action_to_remap);
};

#endif