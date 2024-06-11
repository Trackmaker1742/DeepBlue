#include "input.h"

void Input::init()
{
    // Initialize buttons and hold states
    for (int i = 0; i < 15; i++)
    {
        button.push_back(false);
        hold.push_back(true);
    }

    // Check for controller support
    if (SDL_NumJoysticks() < 1)
        std::cout << "No controller connected!\n";
    else
    {
        controller = SDL_GameControllerOpen(0);
        // Open controller devices
        if (controller == NULL)
            std::cout << "Failed to open controller: %s\n", SDL_GetError();
        else
            std::cout << "Controller connected!\n";
    }
}

bool Input::input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                return true;
                break;
            case SDLK_w:
                button[0] = true;
                break;
            case SDLK_s:
                button[1] = true;
                break;
            case SDLK_a:
                button[2] = true;
                break;
            case SDLK_d:
                button[3] = true;
                break;
            case SDLK_k:
                button[4] = true;
                break;
            case SDLK_l:
                button[5] = true;
                break;
            case SDLK_j:
                button[6] = true;
                break;
            case SDLK_u:
                button[7] = true;
                break;
            case SDLK_g:
                button[8] = true;
                break;
            case SDLK_h:
                button[9] = true;
                break;
            case SDLK_p:
                button[10] = true;
                break;
            case SDLK_UP:
                button[11] = true;
                break;
            case SDLK_DOWN:
                button[12] = true;
                break;
            case SDLK_LEFT:
                button[13] = true;
                break;
            case SDLK_RIGHT:
                button[14] = true;
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
                button[0] = false;
                hold[0] = true;
                break;
            case SDLK_s:
                button[1] = false;
                hold[1] = true;
                break;
            case SDLK_a:
                button[2] = false;
                hold[2] = true;
                break;
            case SDLK_d:
                button[3] = false;
                hold[3] = true;
                break;
            case SDLK_k:
                button[4] = false;
                hold[4] = true;
                break;
            case SDLK_l:
                button[5] = false;
                hold[5] = true;
                break;
            case SDLK_j:
                button[6] = false;
                hold[6] = true;
                break;
            case SDLK_u:
                button[7] = false;
                hold[7] = true;
                break;
            case SDLK_g:
                button[8] = false;
                hold[8] = true;
                break;
            case SDLK_h:
                button[9] = false;
                hold[9] = true;
                break;
            case SDLK_p:
                button[10] = false;
                hold[10] = true;
                break;
            case SDLK_UP:
                button[11] = false;
                hold[11] = true;
                break;
            case SDLK_DOWN:
                button[12] = false;
                hold[12] = true;
                break;
            case SDLK_LEFT:
                button[13] = false;
                hold[13] = true;
                break;
            case SDLK_RIGHT:
                button[14] = false;
                hold[14] = true;
                break;
            }
            break;
        case SDL_CONTROLLERBUTTONDOWN:
            switch (event.cbutton.button)
            {
            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                button[0] = true;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                button[1] = true;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                button[2] = true;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                button[3] = true;
                break;
            case SDL_CONTROLLER_BUTTON_A:
                button[4] = true;
                break;
            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                button[5] = true;
                break;
            case SDL_CONTROLLER_BUTTON_X:
                button[6] = true;
                break;
            case SDL_CONTROLLER_BUTTON_Y:
                button[7] = true;
                break;
            case SDL_CONTROLLER_BUTTON_B:
                button[8] = true;
                break;
            }
            break;
        case SDL_CONTROLLERBUTTONUP:
            switch (event.cbutton.button)
            {
            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                button[0] = false;
                hold[0] = true;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                button[1] = false;
                hold[1] = true;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                button[2] = false;
                hold[2] = true;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                button[3] = false;
                hold[3] = true;
                break;
            case SDL_CONTROLLER_BUTTON_A:
                button[4] = false;
                hold[4] = true;
                break;
            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                button[5] = false;
                hold[5] = true;
                break;
            case SDL_CONTROLLER_BUTTON_X:
                button[6] = false;
                hold[6] = true;
                break;
            case SDL_CONTROLLER_BUTTON_Y:
                button[7] = false;
                hold[7] = true;
                break;
            case SDL_CONTROLLER_BUTTON_B:
                button[8] = false;
                hold[8] = true;
                break;
            }
            break;
        }
    }

    return false;  
}

// Setters
void Input::setHold(uint8_t i, bool state)
{
    hold[i] = state;
}

// Getters
bool Input::getPress(uint8_t i)
{
    return button[i] && hold[i];
}