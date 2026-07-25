#ifndef SMP_CONFIG_H
#define SMP_CONFIG_H

typedef struct
{
    int cursor_deadzone;
    int cursor_max_speed;

    float cursor_exponent;
    float cursor_sensitivity;
    float cursor_smoothing;

    unsigned int scroll_initial_delay_ms;
    unsigned int scroll_repeat_interval_ms;
    int scroll_step;
} ControllerConfig;

extern ControllerConfig g_config;

void config_set_defaults(void);

#endif /* SMP_CONFIG_H */