#include "uinput.h"

#include "config.h"
#include "log.h"

#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

static bool left_button_pressed = false;
static bool right_button_pressed = false;
static bool middle_button_pressed = false;

static bool emit_event(
    int file_descriptor,
    uint16_t type,
    uint16_t code,
    int32_t value)
{
    struct input_event event;

    memset(&event, 0, sizeof(event));

    event.type = type;
    event.code = code;
    event.value = value;

    ssize_t written =
        write(
            file_descriptor,
            &event,
            sizeof(event));

    if (written != (ssize_t)sizeof(event))
    {
        LOG_ERROR(
            "Falha ao enviar evento uinput "
            "(tipo=%u, código=%u, valor=%d): %s",
            type,
            code,
            value,
            strerror(errno));

        return false;
    }

    return true;
}

static bool sync_events(int file_descriptor)
{
    return emit_event(
        file_descriptor,
        EV_SYN,
        SYN_REPORT,
        0);
}

static bool configure_event_type(
    int file_descriptor,
    unsigned long request,
    int value,
    const char *description)
{
    if (ioctl(file_descriptor, request, value) < 0)
    {
        LOG_ERROR(
            "Falha ao configurar %s no uinput: %s",
            description,
            strerror(errno));

        return false;
    }

    return true;
}

static void send_button_state(
    int file_descriptor,
    uint16_t button_code,
    bool pressed,
    bool *previous_state)
{
    if (file_descriptor < 0 || previous_state == NULL)
    {
        return;
    }

    /*
     * Evita reenviar o mesmo estado continuamente.
     */
    if (*previous_state == pressed)
    {
        return;
    }

    if (!emit_event(
            file_descriptor,
            EV_KEY,
            button_code,
            pressed ? 1 : 0))
    {
        return;
    }

    if (!sync_events(file_descriptor))
    {
        return;
    }

    *previous_state = pressed;
}

int uinput_create(void)
{
    int file_descriptor =
        open(
            "/dev/uinput",
            O_WRONLY | O_NONBLOCK);

    if (file_descriptor < 0)
    {
        LOG_ERROR(
            "Não foi possível abrir /dev/uinput: %s",
            strerror(errno));

        return -1;
    }

    if (!configure_event_type(
            file_descriptor,
            UI_SET_EVBIT,
            EV_KEY,
            "eventos de botão") ||

        !configure_event_type(
            file_descriptor,
            UI_SET_KEYBIT,
            BTN_LEFT,
            "botão esquerdo") ||

        !configure_event_type(
            file_descriptor,
            UI_SET_KEYBIT,
            BTN_RIGHT,
            "botão direito") ||

        !configure_event_type(
            file_descriptor,
            UI_SET_KEYBIT,
            BTN_MIDDLE,
            "botão do meio") ||

        !configure_event_type(
            file_descriptor,
            UI_SET_EVBIT,
            EV_REL,
            "eventos relativos") ||

        !configure_event_type(
            file_descriptor,
            UI_SET_RELBIT,
            REL_X,
            "movimento horizontal") ||

        !configure_event_type(
            file_descriptor,
            UI_SET_RELBIT,
            REL_Y,
            "movimento vertical") ||

        !configure_event_type(
            file_descriptor,
            UI_SET_RELBIT,
            REL_WHEEL,
            "rolagem vertical"))
    {
        close(file_descriptor);

        return -1;
    }

    struct uinput_setup setup;

    memset(&setup, 0, sizeof(setup));

    setup.id.bustype = BUS_USB;
    setup.id.vendor = DEVICE_VENDOR;
    setup.id.product = DEVICE_PRODUCT;
    setup.id.version = DEVICE_VERSION;

    snprintf(
        setup.name,
        sizeof(setup.name),
        "%s",
        DEVICE_NAME);

    if (ioctl(
            file_descriptor,
            UI_DEV_SETUP,
            &setup) < 0)
    {
        LOG_ERROR(
            "Falha ao configurar o dispositivo virtual: %s",
            strerror(errno));

        close(file_descriptor);

        return -1;
    }

    if (ioctl(file_descriptor, UI_DEV_CREATE) < 0)
    {
        LOG_ERROR(
            "Falha ao criar o dispositivo virtual: %s",
            strerror(errno));

        close(file_descriptor);

        return -1;
    }

    /*
     * Dá ao kernel um pequeno intervalo para registrar
     * completamente o novo dispositivo.
     */
    usleep(100000);

    left_button_pressed = false;
    right_button_pressed = false;
    middle_button_pressed = false;

    LOG_INFO(
        "Dispositivo virtual criado: %s.",
        DEVICE_NAME);

    return file_descriptor;
}

void uinput_destroy(int file_descriptor)
{
    if (file_descriptor < 0)
    {
        return;
    }

    if (ioctl(file_descriptor, UI_DEV_DESTROY) < 0)
    {
        LOG_WARN(
            "Não foi possível destruir corretamente "
            "o dispositivo virtual: %s",
            strerror(errno));
    }
    else
    {
        LOG_INFO(
            "Dispositivo virtual removido.");
    }

    close(file_descriptor);

    left_button_pressed = false;
    right_button_pressed = false;
    middle_button_pressed = false;
}

void uinput_move(
    int file_descriptor,
    int delta_x,
    int delta_y)
{
    if (file_descriptor < 0)
    {
        return;
    }

    if (delta_x == 0 && delta_y == 0)
    {
        return;
    }

    bool success = true;

    if (delta_x != 0)
    {
        success = emit_event(
            file_descriptor,
            EV_REL,
            REL_X,
            delta_x);
    }

    if (success && delta_y != 0)
    {
        success = emit_event(
            file_descriptor,
            EV_REL,
            REL_Y,
            delta_y);
    }

    if (success)
    {
        sync_events(file_descriptor);
    }
}

void uinput_left_button(
    int file_descriptor,
    bool pressed)
{
    send_button_state(
        file_descriptor,
        BTN_LEFT,
        pressed,
        &left_button_pressed);
}

void uinput_right_button(
    int file_descriptor,
    bool pressed)
{
    send_button_state(
        file_descriptor,
        BTN_RIGHT,
        pressed,
        &right_button_pressed);
}

void uinput_middle_button(
    int file_descriptor,
    bool pressed)
{
    send_button_state(
        file_descriptor,
        BTN_MIDDLE,
        pressed,
        &middle_button_pressed);
}

void uinput_scroll(
    int file_descriptor,
    int direction)
{
    if (file_descriptor < 0 || direction == 0)
    {
        return;
    }

    int value = direction > 0 ? 1 : -1;

    if (emit_event(
            file_descriptor,
            EV_REL,
            REL_WHEEL,
            value))
    {
        sync_events(file_descriptor);
    }
}