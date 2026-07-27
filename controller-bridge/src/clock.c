#include "clock.h"

#include <SDL2/SDL.h>

uint64_t clock_now_ms(void)
{
    return (uint64_t)SDL_GetTicks64();
}

void clock_sleep_ms(uint32_t ms)
{
    SDL_Delay(ms);
}