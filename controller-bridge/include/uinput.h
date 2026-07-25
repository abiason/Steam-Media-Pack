#ifndef SMP_UINPUT_H
#define SMP_UINPUT_H

#include <stdbool.h>

#define DEVICE_NAME "Steam Media Pack Controller Bridge"
#define DEVICE_VENDOR 0x1209
#define DEVICE_PRODUCT 0x0001
#define DEVICE_VERSION 0x0001

int uinput_create(void);

void uinput_destroy(int file_descriptor);

void uinput_move(
    int file_descriptor,
    int delta_x,
    int delta_y);

void uinput_left_button(
    int file_descriptor,
    bool pressed);

void uinput_right_button(
    int file_descriptor,
    bool pressed);

void uinput_middle_button(
    int file_descriptor,
    bool pressed);

void uinput_scroll(
    int file_descriptor,
    int direction);

#endif