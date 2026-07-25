#include "controller.h"

#include "config.h"
#include "log.h"

#include <SDL2/SDL.h>

#include <math.h>
#include <stdlib.h>

#define TRIGGER_THRESHOLD 20000

static int calculate_axis_speed(Sint16 value)
{
    int absolute_value = abs((int)value);

    if (absolute_value <= DEADZONE)
    {
        return 0;
    }

    double normalized =
        (double)(absolute_value - DEADZONE) /
        (double)(32767 - DEADZONE);

    if (normalized < 0.0)
    {
        normalized = 0.0;
    }
    else if (normalized > 1.0)
    {
        normalized = 1.0;
    }

    double speed =
        MIN_SPEED +
        normalized * (MAX_SPEED - MIN_SPEED);

    int direction = value < 0 ? -1 : 1;

    return direction * (int)lround(speed);
}

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
    ControllerState *state)
{
    if (controller == NULL || state == NULL)
    {
        return;
    }

    state->move_x = calculate_axis_speed(
        SDL_GameControllerGetAxis(
            controller,
            SDL_CONTROLLER_AXIS_RIGHTX));

    state->move_y = calculate_axis_speed(
        SDL_GameControllerGetAxis(
            controller,
            SDL_CONTROLLER_AXIS_RIGHTY));

    state->actions = INPUT_NONE;

    if (SDL_GameControllerGetButton(
            controller,
            SDL_CONTROLLER_BUTTON_A))
    {
        state->actions |= INPUT_LEFT_CLICK;
    }

    if (SDL_GameControllerGetButton(
            controller,
            SDL_CONTROLLER_BUTTON_B))
    {
        state->actions |= INPUT_RIGHT_CLICK;
    }

    if (SDL_GameControllerGetButton(
            controller,
            SDL_CONTROLLER_BUTTON_X))
    {
        state->actions |= INPUT_MIDDLE_CLICK;
    }

    Sint16 left_trigger =
        SDL_GameControllerGetAxis(
            controller,
            SDL_CONTROLLER_AXIS_TRIGGERLEFT);

    Sint16 right_trigger =
        SDL_GameControllerGetAxis(
            controller,
            SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

    if (left_trigger > TRIGGER_THRESHOLD)
    {
        state->actions |= INPUT_SCROLL_UP;
    }

    if (right_trigger > TRIGGER_THRESHOLD)
    {
        state->actions |= INPUT_SCROLL_DOWN;
    }
}