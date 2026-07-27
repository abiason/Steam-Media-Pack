#ifndef SMP_SCROLL_H
#define SMP_SCROLL_H

#include <stdint.h>

void scroll_reset(void);

void scroll_update(
    int mouse_fd,
    uint32_t actions);

#endif