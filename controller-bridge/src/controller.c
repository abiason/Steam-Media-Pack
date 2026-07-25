#include "controller.h"
#include "cursor.h"

#include "config.h"
#include "log.h"
#include "mapping.h"

#include <SDL2/SDL.h>

#include <math.h>
#include <stdlib.h>

SDL_GameController *controller_open_first(void)
{
    int joystick_count = SDL_NumJoysticks();

    if (joystick_count < 0)
    {
        LOG_ERROR(
            "Falha ao consultar dispositivos SDL: %s",
            SDL_GetError());

        return NULL;
    }

    LOG_DEBUG(
        "Procurando controles. Dispositivos detectados: %d.",
        joystick_count);

    for (int index = 0; index < joystick_count; ++index)
    {
        if (!SDL_IsGameController(index))
        {
            LOG_DEBUG(
                "Dispositivo no índice %d não é reconhecido "
                "como SDL GameController.",
                index);

            continue;
        }

        SDL_GameController *controller =
            SDL_GameControllerOpen(index);

        if (controller == NULL)
        {
            LOG_WARN(
                "Não foi possível abrir o controle no índice %d: %s",
                index,
                SDL_GetError());

            continue;
        }

        const char *name =
            SDL_GameControllerName(controller);

        SDL_Joystick *joystick =
            SDL_GameControllerGetJoystick(controller);

        SDL_JoystickID instance_id = -1;

        if (joystick != NULL)
        {
            instance_id =
                SDL_JoystickInstanceID(joystick);
        }

        LOG_INFO(
            "Controle conectado: %s — ID: %d.",
            name != NULL
                ? name
                : "controle desconhecido",
            (int)instance_id);

        return controller;
    }

    return NULL;
}

void controller_close(SDL_GameController *controller)
{
    if (controller == NULL)
    {
        return;
    }

    const char *name =
        SDL_GameControllerName(controller);

    LOG_DEBUG(
        "Fechando controle: %s.",
        name != NULL
            ? name
            : "controle desconhecido");

    SDL_GameControllerClose(controller);
}

bool controller_is_connected(
    SDL_GameController *controller)
{
    if (controller == NULL)
    {
        return false;
    }

    return SDL_GameControllerGetAttached(controller)
        == SDL_TRUE;
}

void controller_poll(
    SDL_GameController *controller,
    InputState *state)
{
    if (controller == NULL || state == NULL)
    {
        return;
    }

    Sint16 axis_x = SDL_GameControllerGetAxis(
        controller,
        SDL_CONTROLLER_AXIS_LEFTX);

    Sint16 axis_y = SDL_GameControllerGetAxis(
        controller,
        SDL_CONTROLLER_AXIS_LEFTY);

    cursor_update(
        axis_x,
        axis_y,
        &state->cursor);

    state->buttons.actions = INPUT_NONE;

    if (mapping_left_click(controller))
    {
        state->buttons.actions |= INPUT_LEFT_CLICK;
    }

    if (mapping_right_click(controller))
    {
        state->buttons.actions |= INPUT_RIGHT_CLICK;
    }

    if (mapping_middle_click(controller))
    {
        state->buttons.actions |= INPUT_MIDDLE_CLICK;
    }

    if (mapping_scroll_up(controller))
    {
        state->buttons.actions |= INPUT_SCROLL_UP;
    }

    if (mapping_scroll_down(controller))
    {
        state->buttons.actions |= INPUT_SCROLL_DOWN;
    }
}