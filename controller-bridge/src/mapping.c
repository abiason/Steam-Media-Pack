#include "mapping.h"

#include <SDL2/SDL.h>

#define TRIGGER_THRESHOLD 20000

bool mapping_left_click(SDL_GameController *controller)
{
    if (controller == NULL)
    {
        return false;
    }

    return SDL_GameControllerGetButton(
               controller,
               SDL_CONTROLLER_BUTTON_A) != 0;
}

bool mapping_right_click(SDL_GameController *controller)
{
    if (controller == NULL)
    {
        return false;
    }

    return SDL_GameControllerGetButton(
               controller,
               SDL_CONTROLLER_BUTTON_B) != 0;
}

bool mapping_middle_click(SDL_GameController *controller)
{
    if (controller == NULL)
    {
        return false;
    }

    return SDL_GameControllerGetButton(
               controller,
               SDL_CONTROLLER_BUTTON_X) != 0;
}

bool mapping_scroll_up(SDL_GameController *controller)
{
    if (controller == NULL)
    {
        return false;
    }

    Sint16 trigger_value =
        SDL_GameControllerGetAxis(
            controller,
            SDL_CONTROLLER_AXIS_TRIGGERLEFT);

    return trigger_value > TRIGGER_THRESHOLD;
}

bool mapping_scroll_down(SDL_GameController *controller)
{
    if (controller == NULL)
    {
        return false;
    }

    Sint16 trigger_value =
        SDL_GameControllerGetAxis(
            controller,
            SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

    return trigger_value > TRIGGER_THRESHOLD;
}