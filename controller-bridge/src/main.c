#include "clock.h"
#include "config.h"
#include "config_parser.h"
#include "controller.h"
#include "log.h"
#include "scroll.h"
#include "signals.h"
#include "uinput.h"
#include "version.h"

#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MAIN_LOOP_DELAY_MS 8
#define CONTROLLER_RETRY_DELAY_MS 250

/*
 * Envia o estado atual dos botões para o dispositivo
 * virtual de mouse.
 */
static void update_mouse_buttons(
    int mouse_fd,
    const ButtonState *buttons)
{
    if (buttons == NULL)
    {
        return;
    }

    const uint32_t actions = buttons->actions;

    uinput_left_button(
        mouse_fd,
        (actions & INPUT_LEFT_CLICK) != 0);

    uinput_right_button(
        mouse_fd,
        (actions & INPUT_RIGHT_CLICK) != 0);

    uinput_middle_button(
        mouse_fd,
        (actions & INPUT_MIDDLE_CLICK) != 0);
}

/*
 * Libera todos os botões do mouse virtual.
 *
 * Isso evita que algum botão permaneça pressionado
 * depois de uma desconexão ou encerramento.
 */
static void release_mouse_buttons(int mouse_fd)
{
    uinput_left_button(mouse_fd, false);
    uinput_right_button(mouse_fd, false);
    uinput_middle_button(mouse_fd, false);
}

/*
 * Aguarda até que um controle compatível com SDL2
 * esteja disponível.
 *
 * SDL_PumpEvents() atualiza o estado interno da SDL,
 * permitindo que dispositivos conectados novamente
 * apareçam em SDL_NumJoysticks().
 */
static SDL_GameController *wait_for_controller(void)
{
    bool waiting_logged = false;

    while (running)
    {
        /*
         * Atualiza os dispositivos conhecidos pela SDL.
         */
        SDL_PumpEvents();

        /*
         * Faz uma nova varredura nos dispositivos.
         *
         * Um controle reconectado pode receber um novo
         * índice e uma nova instance ID.
         */
        SDL_GameController *controller =
            controller_open_first();

        if (controller != NULL)
        {
            return controller;
        }

        if (!waiting_logged)
        {
            LOG_INFO(
                "Aguardando conexão de um controle compatível.");

            waiting_logged = true;
        }

        clock_sleep_ms(CONTROLLER_RETRY_DELAY_MS);
    }

    return NULL;
}

int main(void)
{
    config_set_defaults();

    config_load(CONFIG_DEFAULT_PATH);

    LOG_INFO(
        "Iniciando %s %s.",
        SMP_CONTROLLER_NAME,
        SMP_CONTROLLER_VERSION);

    /*
     * Instala os manipuladores de SIGINT e SIGTERM.
     *
     * Isso permite encerrar corretamente com Ctrl+C
     * ou através do systemd.
     */
    signals_install();

    /*
     * Inicializa os subsistemas necessários da SDL2.
     */
    if (SDL_Init(
            SDL_INIT_GAMECONTROLLER |
            SDL_INIT_JOYSTICK |
            SDL_INIT_EVENTS) != 0)
    {
        LOG_ERROR(
            "Falha ao inicializar SDL2: %s.",
            SDL_GetError());

        return EXIT_FAILURE;
    }

    /*
     * Mantém habilitada a atualização dos estados
     * de controles e joysticks.
     */
    SDL_GameControllerEventState(SDL_ENABLE);
    SDL_JoystickEventState(SDL_ENABLE);

    /*
     * Cria o dispositivo virtual de mouse através
     * do uinput.
     */
    int mouse_fd = uinput_create();

    if (mouse_fd < 0)
    {
        LOG_ERROR(
            "Não foi possível criar o dispositivo virtual.");

        SDL_Quit();

        return EXIT_FAILURE;
    }

    /*
     * Inicializa o estado interno do módulo de scroll.
     */
    scroll_reset();

    /*
     * Aguarda o primeiro controle.
     */
    SDL_GameController *controller =
        wait_for_controller();

    if (controller == NULL)
    {
        /*
         * Normalmente significa que o programa recebeu
         * um sinal enquanto aguardava um controle.
         */
        release_mouse_buttons(mouse_fd);
        scroll_reset();

        uinput_destroy(mouse_fd);
        SDL_Quit();

        LOG_INFO("Controller Bridge encerrado.");

        return EXIT_SUCCESS;
    }

    /*
     * Estado atual das entradas do controle.
     */
    InputState state = {0};

    while (running)
    {
        /*
         * Atualiza internamente o estado dos dispositivos.
         *
         * Esta chamada também permite detectar quando
         * o controle foi desconectado.
         */
        SDL_PumpEvents();

        /*
         * Verifica se a instância atual ainda está conectada.
         */
        if (!controller_is_connected(controller))
        {
            LOG_WARN("Controle desconectado.");

            controller_close(controller);
            controller = NULL;

            /*
             * Limpa todos os estados associados ao controle
             * anterior.
             */
            state = (InputState){0};

            scroll_reset();
            release_mouse_buttons(mouse_fd);

            LOG_INFO(
                "Aguardando reconexão do controle.");

            /*
             * Faz uma nova varredura até que outro controle
             * compatível seja encontrado.
             */
            controller = wait_for_controller();

            if (controller == NULL)
            {
                break;
            }

            LOG_INFO(
                "Controle reconectado e pronto para uso.");

            continue;
        }

        /*
         * Atualiza:
         *
         * state.cursor.delta_x
         * state.cursor.delta_y
         * state.buttons.actions
         */
        controller_poll(
            controller,
            &state);

        /*
         * Envia movimento apenas quando existe
         * deslocamento no cursor.
         */
        if (state.cursor.delta_x != 0 ||
            state.cursor.delta_y != 0)
        {
            uinput_move(
                mouse_fd,
                state.cursor.delta_x,
                state.cursor.delta_y);
        }

        /*
         * Atualiza clique esquerdo, direito e central.
         */
        update_mouse_buttons(
            mouse_fd,
            &state.buttons);

        /*
         * Atualiza o scroll e sua repetição.
         */
        scroll_update(
            mouse_fd,
            state.buttons.actions);

        /*
         * Aproximadamente 125 atualizações por segundo.
         */
        clock_sleep_ms(MAIN_LOOP_DELAY_MS);
    }

    LOG_INFO(
        "Encerrando Controller Bridge.");

    /*
     * Libera os estados virtuais antes de destruir
     * o dispositivo.
     */
    release_mouse_buttons(mouse_fd);

    scroll_reset();

    if (controller != NULL)
    {
        controller_close(controller);
        controller = NULL;
    }

    uinput_destroy(mouse_fd);

    SDL_Quit();

    LOG_INFO(
        "Controller Bridge encerrado corretamente.");

    return EXIT_SUCCESS;
}