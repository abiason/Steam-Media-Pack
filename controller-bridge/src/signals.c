#include "signals.h"

volatile sig_atomic_t running = 1;

static void handle_signal(int signal_number)
{
    (void)signal_number;
    running = 0;
}

void signals_install(void)
{
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
}