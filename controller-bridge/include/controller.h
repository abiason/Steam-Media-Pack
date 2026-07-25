#ifndef SMP_CONTROLLER_H
#define SMP_CONTROLLER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

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
    int move_x;
    int move_y;

    unsigned int actions;

} ControllerState;

SDL_GameController *controller_open_first(void);

void controller_close(SDL_GameController *);

bool controller_is_connected(SDL_GameController *);

void controller_poll(
    SDL_GameController *,
    ControllerState *);

#endif