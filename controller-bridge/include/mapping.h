#ifndef SMP_MAPPING_H
#define SMP_MAPPING_H

#include <SDL2/SDL.h>
#include <stdbool.h>

bool mapping_left_click(SDL_GameController *controller);

bool mapping_right_click(SDL_GameController *controller);

bool mapping_middle_click(SDL_GameController *controller);

bool mapping_scroll_up(SDL_GameController *controller);

bool mapping_scroll_down(SDL_GameController *controller);

#endif