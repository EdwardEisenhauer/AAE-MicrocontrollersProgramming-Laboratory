#ifndef CLOCK_H_
#define CLOCK_H_

#include "main.h"

typedef enum {
  PLLP_DIV_2 = 0b00,
  PLLP_DIV_4 = 0b01,
  PLLP_DIV_6 = 0b10,
  PLLP_DIV_8 = 0b11
} PLLP_Division;

#define HSI_VALUE     16000000UL // High-Speed Internal clock frequency (16 MHz)
#define HSE_VALUE      8000000UL // High-Speed External clock frequency (8 MHz)
#define PLL_MAX_FREQ 100000000UL // Phase-Locked Loop maximum frequency (100 MHz)

int setup_systick(uint32_t);
void setup_hsi ();
void disable_hsi();
void setup_hse();
void setup_pll(unsigned, unsigned, unsigned, unsigned, PLLP_Division);
void setup_pll_max();
void setup_pll_80mhz();
void wait_ticks(uint32_t);
int wait_ms(uint32_t);

#endif /* CLOCK_H_ */