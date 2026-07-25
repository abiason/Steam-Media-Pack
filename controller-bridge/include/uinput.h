#ifndef SMP_UINPUT_H
#define SMP_UINPUT_H

#include <stdbool.h>

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