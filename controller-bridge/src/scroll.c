#include "scroll.h"

#include "clock.h"
#include "config.h"
#include "controller.h"
#include "uinput.h"

#include <stdbool.h>

typedef struct
{
    bool pressed;
    uint64_t pressed_at;
    uint64_t last_repeat;
} ScrollButtonState;

static ScrollButtonState scroll_up_state;
static ScrollButtonState scroll_down_state;

static void update_direction(
    int mouse_fd,
    bool active,
    int direction,
    ScrollButtonState *state)
{
    uint64_t now = clock_now_ms();

    if (!active)
    {
        state->pressed = false;
        state->pressed_at = 0;
        state->last_repeat = 0;
        return;
    }

    if (!state->pressed)
    {
        state->pressed = true;
        state->pressed_at = now;
        state->last_repeat = now;

        uinput_scroll(
            mouse_fd,
            direction * g_config.scroll_step);

        return;
    }

    if ((now - state->pressed_at) <
        g_config.scroll_initial_delay_ms)
    {
        return;
    }

    if ((now - state->last_repeat) >=
        g_config.scroll_repeat_interval_ms)
    {
        state->last_repeat = now;

        uinput_scroll(
            mouse_fd,
            direction * g_config.scroll_step);
    }
}

void scroll_reset(void)
{
    scroll_up_state = (ScrollButtonState){0};
    scroll_down_state = (ScrollButtonState){0};
}

void scroll_update(
    int mouse_fd,
    uint32_t actions)
{
    update_direction(
        mouse_fd,
        (actions & INPUT_SCROLL_UP) != 0,
        1,
        &scroll_up_state);

    update_direction(
        mouse_fd,
        (actions & INPUT_SCROLL_DOWN) != 0,
        -1,
        &scroll_down_state);
}