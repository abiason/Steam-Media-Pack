#include <SDL2/SDL.h>

#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEVICE_NAME "Steam Media Pack Controller Bridge"
#define DEADZONE 7000
#define MIN_SPEED 2.0
#define MAX_SPEED 16.0
#define LOOP_DELAY_MS 8

static volatile sig_atomic_t running = 1;

static void handle_signal(int signal_number)
{
    (void)signal_number;
    running = 0;
}

static int emit_event(int fd, int type, int code, int value)
{
    struct input_event event;

    memset(&event, 0, sizeof(event));
    event.type = type;
    event.code = code;
    event.value = value;

    if (write(fd, &event, sizeof(event)) < 0) {
        perror("write");
        return -1;
    }

    return 0;
}

static int sync_events(int fd)
{
    return emit_event(fd, EV_SYN, SYN_REPORT, 0);
}

static int create_virtual_mouse(void)
{
    int fd;
    struct uinput_setup setup;

    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if (fd < 0) {
        perror("Não foi possível abrir /dev/uinput");
        return -1;
    }

    if (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0 ||
        ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0 ||
        ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT) < 0 ||
        ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE) < 0 ||
        ioctl(fd, UI_SET_EVBIT, EV_REL) < 0 ||
        ioctl(fd, UI_SET_RELBIT, REL_X) < 0 ||
        ioctl(fd, UI_SET_RELBIT, REL_Y) < 0 ||
        ioctl(fd, UI_SET_RELBIT, REL_WHEEL) < 0) {
        perror("Falha ao configurar uinput");
    close(fd);
    return -1;
        }

        memset(&setup, 0, sizeof(setup));

        snprintf(setup.name, UINPUT_MAX_NAME_SIZE, "%s", DEVICE_NAME);
        setup.id.bustype = BUS_USB;
        setup.id.vendor = 0x1209;
        setup.id.product = 0x0001;
        setup.id.version = 1;

        if (ioctl(fd, UI_DEV_SETUP, &setup) < 0) {
            perror("UI_DEV_SETUP");
            close(fd);
            return -1;
        }

        if (ioctl(fd, UI_DEV_CREATE) < 0) {
            perror("UI_DEV_CREATE");
            close(fd);
            return -1;
        }

        sleep(1);

        return fd;
}

static SDL_GameController *open_first_controller(void)
{
    int joystick_count = SDL_NumJoysticks();

    for (int index = 0; index < joystick_count; index++) {
        if (!SDL_IsGameController(index)) {
            continue;
        }

        SDL_GameController *controller = SDL_GameControllerOpen(index);

        if (controller != NULL) {
            printf(
                "Controle conectado: %s\n",
                SDL_GameControllerName(controller)
            );

            return controller;
        }
    }

    return NULL;
}

static int calculate_axis_speed(Sint16 axis_value)
{
    int absolute_value = abs(axis_value);

    if (absolute_value <= DEADZONE) {
        return 0;
    }

    double normalized =
    (double)(absolute_value - DEADZONE) /
    (double)(32767 - DEADZONE);

    double speed =
    MIN_SPEED + normalized * (MAX_SPEED - MIN_SPEED);

    int direction = axis_value < 0 ? -1 : 1;

    return direction * (int)lround(speed);
}

static void send_button_event(int fd, int button, bool pressed)
{
    emit_event(fd, EV_KEY, button, pressed ? 1 : 0);
    sync_events(fd);
}

int main(void)
{
    int mouse_fd;
    SDL_GameController *controller = NULL;
    bool left_pressed = false;
    bool right_pressed = false;
    bool middle_pressed = false;

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "Erro ao inicializar SDL2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    mouse_fd = create_virtual_mouse();

    if (mouse_fd < 0) {
        SDL_Quit();
        return EXIT_FAILURE;
    }

    printf("Controller Bridge iniciado.\n");

    while (running) {
        SDL_PumpEvents();

        if (controller == NULL) {
            controller = open_first_controller();

            if (controller == NULL) {
                SDL_Delay(1000);
                continue;
            }
        }

        if (!SDL_GameControllerGetAttached(controller)) {
            printf("Controle desconectado.\n");
            SDL_GameControllerClose(controller);
            controller = NULL;
            SDL_Delay(500);
            continue;
        }

        Sint16 right_x = SDL_GameControllerGetAxis(
            controller,
            SDL_CONTROLLER_AXIS_RIGHTX
        );

        Sint16 right_y = SDL_GameControllerGetAxis(
            controller,
            SDL_CONTROLLER_AXIS_RIGHTY
        );

        int movement_x = calculate_axis_speed(right_x);
        int movement_y = calculate_axis_speed(right_y);

        if (movement_x != 0 || movement_y != 0) {
            emit_event(mouse_fd, EV_REL, REL_X, movement_x);
            emit_event(mouse_fd, EV_REL, REL_Y, movement_y);
            sync_events(mouse_fd);
        }

        bool current_left = SDL_GameControllerGetButton(
            controller,
            SDL_CONTROLLER_BUTTON_A
        );

        bool current_right = SDL_GameControllerGetButton(
            controller,
            SDL_CONTROLLER_BUTTON_B
        );

        bool current_middle = SDL_GameControllerGetButton(
            controller,
            SDL_CONTROLLER_BUTTON_X
        );

        if (current_left != left_pressed) {
            left_pressed = current_left;
            send_button_event(mouse_fd, BTN_LEFT, left_pressed);
        }

        if (current_right != right_pressed) {
            right_pressed = current_right;
            send_button_event(mouse_fd, BTN_RIGHT, right_pressed);
        }

        if (current_middle != middle_pressed) {
            middle_pressed = current_middle;
            send_button_event(mouse_fd, BTN_MIDDLE, middle_pressed);
        }

        Sint16 left_trigger = SDL_GameControllerGetAxis(
            controller,
            SDL_CONTROLLER_AXIS_TRIGGERLEFT
        );

        Sint16 right_trigger = SDL_GameControllerGetAxis(
            controller,
            SDL_CONTROLLER_AXIS_TRIGGERRIGHT
        );

        if (left_trigger > 20000) {
            emit_event(mouse_fd, EV_REL, REL_WHEEL, 1);
            sync_events(mouse_fd);
        }

        if (right_trigger > 20000) {
            emit_event(mouse_fd, EV_REL, REL_WHEEL, -1);
            sync_events(mouse_fd);
        }

        SDL_Delay(LOOP_DELAY_MS);
    }

    if (left_pressed) {
        send_button_event(mouse_fd, BTN_LEFT, false);
    }

    if (right_pressed) {
        send_button_event(mouse_fd, BTN_RIGHT, false);
    }

    if (middle_pressed) {
        send_button_event(mouse_fd, BTN_MIDDLE, false);
    }

    if (controller != NULL) {
        SDL_GameControllerClose(controller);
    }

    ioctl(mouse_fd, UI_DEV_DESTROY);
    close(mouse_fd);

    SDL_Quit();

    printf("Controller Bridge encerrado.\n");

    return EXIT_SUCCESS;
}
