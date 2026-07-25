#include "config.h"
#include "controller.h"
#include "log.h"
#include "signals.h"
#include "uinput.h"
#include "version.h"

#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdlib.h>

int main(void)
{
    SDL_GameController *controller = NULL;
    ControllerState state = {0};

    int mouse_fd = -1;

    log_set_level(LOG_LEVEL_INFO);

    LOG_INFO(
        "Iniciando Steam Media Pack Controller Bridge %s.",
        SMP_CONTROLLER_VERSION);

    if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0)
    {
        LOG_ERROR(
            "Falha ao inicializar SDL2: %s",
            SDL_GetError());

        return EXIT_FAILURE;
    }

    signals_install();

    mouse_fd = uinput_create();

    if (mouse_fd < 0)
    {
        LOG_ERROR(
            "Não foi possível criar o dispositivo virtual de entrada.");

        SDL_Quit();

        return EXIT_FAILURE;
    }

    while (running)
    {
        SDL_PumpEvents();

        if (controller == NULL)
        {
            controller = controller_open_first();

            if (controller == NULL)
            {
                SDL_Delay(1000);
                continue;
            }
        }

        if (!controller_is_connected(controller))
        {
            LOG_WARN("Controle desconectado.");

            /*
             * Libera possíveis botões virtuais que tenham
             * permanecido pressionados no momento da desconexão.
             */
            uinput_left_button(mouse_fd, false);
            uinput_right_button(mouse_fd, false);
            uinput_middle_button(mouse_fd, false);

            controller_close(controller);
            controller = NULL;

            SDL_Delay(500);
            continue;
        }

        controller_poll(controller, &state);

        if (state.move_x != 0 || state.move_y != 0)
        {
            uinput_move(
                mouse_fd,
                state.move_x,
                state.move_y);
        }

        uinput_left_button(
            mouse_fd,
            (state.actions & INPUT_LEFT_CLICK) != 0);

        uinput_right_button(
            mouse_fd,
            (state.actions & INPUT_RIGHT_CLICK) != 0);

        uinput_middle_button(
            mouse_fd,
            (state.actions & INPUT_MIDDLE_CLICK) != 0);

        if ((state.actions & INPUT_SCROLL_UP) != 0)
        {
            uinput_scroll(mouse_fd, 1);
        }

        if ((state.actions & INPUT_SCROLL_DOWN) != 0)
        {
            uinput_scroll(mouse_fd, -1);
        }

        SDL_Delay(LOOP_DELAY_MS);
    }

    LOG_INFO("Encerramento solicitado.");

    /*
     * Garante que nenhum botão virtual permaneça pressionado
     * depois do encerramento do processo.
     */
    uinput_left_button(mouse_fd, false);
    uinput_right_button(mouse_fd, false);
    uinput_middle_button(mouse_fd, false);

    controller_close(controller);
    uinput_destroy(mouse_fd);

    SDL_Quit();

    LOG_INFO("Controller Bridge encerrado.");

    return EXIT_SUCCESS;
}