#include "uinput.h"
#include "config.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

static void sync_events(int fd)
{
    emit_event(fd, EV_SYN, SYN_REPORT, 0);
}

int uinput_create(void)
{
    int fd;
    struct uinput_setup setup;

    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if (fd < 0) {
        perror("Não foi possível abrir /dev/uinput");
        return -1;
    }

    ioctl(fd, UI_SET_EVBIT, EV_KEY);

    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE);

    ioctl(fd, UI_SET_EVBIT, EV_REL);

    ioctl(fd, UI_SET_RELBIT, REL_X);
    ioctl(fd, UI_SET_RELBIT, REL_Y);
    ioctl(fd, UI_SET_RELBIT, REL_WHEEL);

    memset(&setup, 0, sizeof(setup));

    snprintf(
        setup.name,
        UINPUT_MAX_NAME_SIZE,
        "%s",
        DEVICE_NAME
    );

    setup.id.bustype = BUS_USB;
    setup.id.vendor  = DEVICE_VENDOR;
    setup.id.product = DEVICE_PRODUCT;
    setup.id.version = DEVICE_VERSION;

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

void uinput_destroy(int fd)
{
    if (fd < 0)
        return;

    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
}

void uinput_move(int fd, int dx, int dy)
{
    if (dx != 0)
        emit_event(fd, EV_REL, REL_X, dx);

    if (dy != 0)
        emit_event(fd, EV_REL, REL_Y, dy);

    if (dx != 0 || dy != 0)
        sync_events(fd);
}

void uinput_left_button(int fd, bool pressed)
{
    emit_event(fd, EV_KEY, BTN_LEFT, pressed);
    sync_events(fd);
}

void uinput_right_button(int fd, bool pressed)
{
    emit_event(fd, EV_KEY, BTN_RIGHT, pressed);
    sync_events(fd);
}

void uinput_middle_button(int fd, bool pressed)
{
    emit_event(fd, EV_KEY, BTN_MIDDLE, pressed);
    sync_events(fd);
}

void uinput_scroll(int fd, int amount)
{
    emit_event(fd, EV_REL, REL_WHEEL, amount);
    sync_events(fd);
}