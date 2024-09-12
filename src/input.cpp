#include "input.h"

void Input::init() 
{
    button.resize(static_cast<int>(Action::ACTION_MAX), false);
    hold.resize(static_cast<int>(Action::ACTION_MAX), true);

    // Default keybind
    remapKey(SDLK_w, Action::MOVE_UP);
    remapKey(SDLK_s, Action::MOVE_DOWN);
    remapKey(SDLK_a, Action::MOVE_LEFT);
    remapKey(SDLK_d, Action::MOVE_RIGHT);
    remapKey(SDLK_j, Action::ACTION1);
    remapKey(SDLK_k, Action::ACTION2);

    // Default controller bind
    remapControllerButton(SDL_CONTROLLER_BUTTON_DPAD_UP, Action::MOVE_UP);
    remapControllerButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN, Action::MOVE_DOWN);
    remapControllerButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT, Action::MOVE_LEFT);
    remapControllerButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, Action::MOVE_RIGHT);
    remapControllerButton(SDL_CONTROLLER_BUTTON_A, Action::ACTION1);
    remapControllerButton(SDL_CONTROLLER_BUTTON_B, Action::ACTION2);
}
bool Input::handleInput() 
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return true;
            case SDL_KEYDOWN:
                handleKeyboardEvent(event.key);
                break;
            case SDL_KEYUP:
                handleKeyboardEvent(event.key);
                break;
            // case SDL_CONTROLLERBUTTONDOWN:
            //     handleControllerButtonEvent(event.cbutton);
            //     break;
            // case SDL_CONTROLLERBUTTONUP:
            //     handleControllerButtonEvent(event.cbutton);
            //     break;
        }
    }
    return false;
}

void Input::setHold(Action action, bool hol)
{
    hold[static_cast<int>(action)] = false;
}
bool Input::getPress(Action action)
{
    return button[static_cast<int>(action)] && hold[static_cast<int>(action)];
}

void Input::handleKeyboardEvent(const SDL_KeyboardEvent &keyEvent) 
{
    auto it = key_to_action.find(keyEvent.keysym.sym);
    if (it != key_to_action.end()) 
    {
        Action action = it->second;
        bool key_pressed = (keyEvent.type == SDL_KEYDOWN);
        button[static_cast<int>(action)] = key_pressed;
        if (!key_pressed) hold[static_cast<int>(action)] = true;
    }
}
// void Input::handleControllerButtonEvent(const SDL_ControllerButtonEvent &buttonEvent) {
//     auto it = controller_button_to_action.find(buttonEvent.button);
//     if (it != controller_button_to_action.end()) 
//     {
//         Action action = it->second;
//         bool button_pressed = (buttonEvent.type == SDL_CONTROLLERBUTTONDOWN);
//         button[static_cast<int>(action)] = button_pressed;
//     }
// }

void Input::remapKey(SDL_Keycode key, Action action) 
{
    key_to_action[key] = action;
}
void Input::remapControllerButton(SDL_GameControllerButton button, Action action) 
{
    controller_button_to_action[button] = action;
}
void Input::waitForKeyRemap(Action action_to_remap) 
{
    std::cout << "Press the new key for action: " << static_cast<int>(action_to_remap) << "\n";
    SDL_Event event;

    // Wait for user to press a key
    bool key_remapped = false;
    while (!key_remapped) 
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_KEYDOWN) 
            {
                SDL_Keycode new_key = event.key.keysym.sym;

                // Check if the action was previously mapped to a different key
                auto action_it = std::find_if(key_to_action.begin(), key_to_action.end(),
                    [action_to_remap](const auto& pair) { return pair.second == action_to_remap; });
                if (action_it != key_to_action.end()) 
                {
                    // Remove the previous key mapping for this action
                    key_to_action.erase(action_it);
                }

                // Check if the new key is already mapped to another action
                auto key_it = key_to_action.find(new_key);
                if (key_it != key_to_action.end()) 
                {
                    // Remove the previous mapping for this key
                    key_to_action.erase(key_it);
                }

                // Remap the new key to the specified action
                remapKey(new_key, action_to_remap);
                key_remapped = true;
            }
        }
        SDL_Delay(100); // Small delay to avoid high CPU usage
    }
}
