#include "config.h"

ControllerConfig g_config;

void config_set_defaults(void)
{
    g_config.cursor_deadzone = 8000;
    g_config.cursor_max_speed = 20;

    g_config.cursor_exponent = 2.2f;
    g_config.cursor_sensitivity = 1.0f;
    g_config.cursor_smoothing = 0.75f;

    g_config.scroll_initial_delay_ms = 250;
    g_config.scroll_repeat_interval_ms = 75;
    g_config.scroll_step = 1;
}