#ifndef SMP_CURSOR_H
#define SMP_CURSOR_H

#include <SDL2/SDL.h>

typedef struct
{
    int delta_x;
    int delta_y;
} CursorState;

void cursor_update(
    Sint16 axis_x,
    Sint16 axis_y,
    CursorState *state);

#endif