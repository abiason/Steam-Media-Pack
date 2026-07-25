#ifndef SMP_UINPUT_H
#define SMP_UINPUT_H

#include <stdbool.h>

int uinput_create(void);
void uinput_destroy(int fd);

void uinput_move(int fd, int dx, int dy);

void uinput_left_button(int fd, bool pressed);
void uinput_right_button(int fd, bool pressed);
void uinput_middle_button(int fd, bool pressed);

void uinput_scroll(int fd, int amount);

#endif