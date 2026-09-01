#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_init(uint32_t hz);
uint32_t get_ticks(void);
void sleep_ms(uint32_t ms);

#endif
