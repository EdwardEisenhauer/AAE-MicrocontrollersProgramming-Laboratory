#include "clock.h"

uint32_t core_clock = HSI_VALUE;
volatile uint32_t systick_counter = 0;

void SysTick_Handler () {
	if ( systick_counter ) --systick_counter;
}

int setup_systick(uint32_t ticks) {
	// ticks must fit into 24-bit LOAD register.
	if ( ticks == 0 || ticks > 0xFFFFFF ) {
		return -1;
	}
	// SysTick Control and Status Register
	SysTick->CTRL = 0;
	// SysTick Reload Value Register
	SysTick->LOAD = ticks - 1;
	// SysTick Current Value Register
	SysTick->VAL = 0;

	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
									SysTick_CTRL_TICKINT_Msk   |
									SysTick_CTRL_ENABLE_Msk;
	return 0;
}

// Set up High-Speed Internal clock
// f_HSI = 16 MHz
void setup_hsi () {
	if ( (RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_HSI) return;

	RCC->CR |= RCC_CR_HSION;  // Enable HSI clock
	while ( !(RCC->CR & RCC_CR_HSIRDY) );  // Wait for the HSI clock to be ready
	// Set HSI as the system clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	core_clock = HSI_VALUE;
}

void wait_ticks (uint32_t ticks) {
	systick_counter = ticks / SYSTICK_TICKS;
	while ( systick_counter );
}

int wait_ms (uint32_t ms) {
	if ( ms > 0xFFFFFF ) {
		return -1;
	}
	wait_ticks((uint32_t)(ms * (core_clock / 1000UL)));
	return 0;
}
