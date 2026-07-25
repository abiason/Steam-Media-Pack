#ifndef SMP_CLOCK_H
#define SMP_CLOCK_H

#include <stdint.h>

uint64_t clock_now_ms(void);

void clock_sleep_ms(uint32_t ms);

#endif /* SMP_CLOCK_H */