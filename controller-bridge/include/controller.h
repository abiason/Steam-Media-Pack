#ifndef SMP_CONTROLLER_H
#define SMP_CONTROLLER_H

#include "cursor.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#include "input.h"

SDL_GameController *controller_open_first(void);

void controller_close(SDL_GameController *);

bool controller_is_connected(SDL_GameController *);

void controller_poll(
    SDL_GameController *,
    InputState *);

#endif