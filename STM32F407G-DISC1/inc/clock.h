#ifndef CLOCK_H_
#define CLOCK_H_

#include "main.h"

#define HSI_VALUE     16000000UL // High-Speed Internal clock frequency (16 MHz)
#define HSE_VALUE      8000000UL // High-Speed External clock frequency (8 MHz)
#define PLL_MAX_FREQ 100000000UL // Phase-Locked Loop maximum frequency (100 MHz)

int setup_systick(uint32_t);
void setup_hsi ();
void wait_ticks(uint32_t);
int wait_ms(uint32_t);

#endif /* CLOCK_H_ */