#include "cursor.h"
#include "config.h"

#include <math.h>

static int apply_smoothing(
    float *history,
    int current);

static float smooth_x = 0.0f;
static float smooth_y = 0.0f;

static float normalize_axis(Sint16 axis)
{
    if (axis >= 0)
    {
        return (float)axis / 32767.0f;
    }

    return (float)axis / 32768.0f;
}

static float apply_deadzone(float value)
{
    const float deadzone =
        (float)g_config.cursor_deadzone / 32767.0f;

    float magnitude = fabsf(value);

    if (magnitude <= deadzone)
    {
        return 0.0f;
    }

    magnitude =
        (magnitude - deadzone) /
        (1.0f - deadzone);

    return copysignf(magnitude, value);
}

static float apply_curve(float value)
{
    float magnitude = powf(
        fabsf(value),
        g_config.cursor_exponent);

    return copysignf(magnitude, value);
}

static int apply_speed(float value)
{
    float speed =
        value *
        g_config.cursor_max_speed *
        g_config.cursor_sensitivity;

    if (speed > g_config.cursor_max_speed)
    {
        speed = g_config.cursor_max_speed;
    }
    else if (speed < -g_config.cursor_max_speed)
    {
        speed = -g_config.cursor_max_speed;
    }

    if (speed > 0.0f && speed < 1.0f)
    {
        return 1;
    }

    if (speed < 0.0f && speed > -1.0f)
    {
        return -1;
    }

    return (int)lroundf(speed);
}

void cursor_update(
    Sint16 axis_x,
    Sint16 axis_y,
    CursorState *state)
{
    if (state == NULL)
    {
        return;
    }

    float x = normalize_axis(axis_x);
    float y = normalize_axis(axis_y);

    x = apply_deadzone(x);
    y = apply_deadzone(y);

    x = apply_curve(x);
    y = apply_curve(y);

    int speed_x = apply_speed(x);
    int speed_y = apply_speed(y);

    state->delta_x =
        apply_smoothing(
            &smooth_x,
            speed_x);

    state->delta_y =
        apply_smoothing(
            &smooth_y,
            speed_y);
}

static int apply_smoothing(
    float *history,
    int current)
{
    *history =
        (*history * g_config.cursor_smoothing) +
        ((float)current * (1.0f - g_config.cursor_smoothing));

    return (int)lroundf(*history);
}