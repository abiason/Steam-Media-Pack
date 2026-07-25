#ifndef SMP_INPUT_H
#define SMP_INPUT_H

#include <stdint.h>

#include "cursor.h"

typedef enum
{
    INPUT_NONE          = 0,

    INPUT_LEFT_CLICK    = 1 << 0,
    INPUT_RIGHT_CLICK   = 1 << 1,
    INPUT_MIDDLE_CLICK  = 1 << 2,

    INPUT_SCROLL_UP     = 1 << 3,
    INPUT_SCROLL_DOWN   = 1 << 4

} InputActions;

typedef struct
{
    uint32_t actions;
} ButtonState;

typedef struct
{
    CursorState cursor;
    ButtonState buttons;
} InputState;

#endif /* SMP_INPUT_H */