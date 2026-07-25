#ifndef SMP_SIGNALS_H
#define SMP_SIGNALS_H

#include <signal.h>

extern volatile sig_atomic_t running;

void signals_install(void);

#endif