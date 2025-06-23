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

// Disable High-Speed Internal clock
void disable_hsi () {
	// Disable HSI clock
	RCC->CR &= ~RCC_CR_HSION;
	// Wait for the HSI clock to be disabled
	while (RCC->CR & RCC_CR_HSIRDY);
}

// Set up High-Speed External clock
// f_HSE = 8 MHz
void setup_hse () {
	if ( (RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_HSE) return;

	RCC->CR |= RCC_CR_HSEON;  // Enable HSE clock
	while ( !(RCC->CR & RCC_CR_HSERDY) );  // Wait for the HSE clock to be ready
	// Set HSE as the system clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSE;
	// Wait for the system clock to switch to HSE
	while ( !(RCC->CFGR & RCC_CFGR_SWS_HSE) );
	core_clock = HSE_VALUE;
}

// Set up Phase Locked Loop
void setup_pll (
	unsigned wait_states,
	unsigned pll_src,
	unsigned pllm,
	unsigned plln,
	PLLP_Division pllp
) {
	if (pllm < 2 || pllm > 63 || plln < 50 || plln > 432) {
			while (1);
	}

	FLASH->ACR |= wait_states;
	// Check that the new number of wait states is taken into account to access
	// the Flash memory
	while ( !(FLASH->ACR & wait_states) );

	// HSE must be ready
	setup_hse();

	RCC->CR &= ~RCC_CR_PLLON;  // Disable the PLL
	while (RCC->CR & RCC_CR_PLLRDY);

	// RCC PLL configuration register
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;             // Clear PLLSRC
	RCC->PLLCFGR |= pll_src;                         // Set PLL source

	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;               // Clear PLLM
	RCC->PLLCFGR |= (pllm << RCC_PLLCFGR_PLLM_Pos);  // Set PLLM

	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;               // Clear PLLN
	RCC->PLLCFGR |= (plln << RCC_PLLCFGR_PLLN_Pos);  // Set PLLN

	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP;               // Clear PLLP
	RCC->PLLCFGR |= (pllp << RCC_PLLCFGR_PLLP_Pos);  // Set PLLP

	RCC->CR |= RCC_CR_PLLON;  // Enable the PLL
	while ( !(RCC->CR & RCC_CR_PLLRDY) );

	// Select PLL as the system clock
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	// Wait until PLL is system clock
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	core_clock = HSE_VALUE * plln / pllm / ( 2 * (pllp + 1) ) ;
}

// Set up Phase Locked Loop to get the maximum system clock
// For STM32F411 it is 100 MHz
void setup_pll_max () {
	// For f_HCLK = 80 MHz at 3.3 V the number of wait states ust be set to 3
	unsigned wait_states = FLASH_ACR_LATENCY_3WS;
	// HSE source on STM32F411E-DISCO is 8 MHz
	unsigned pll_src = RCC_PLLCFGR_PLLSRC_HSE;
	unsigned pllm = 4;  // Set PLLM to 4
	unsigned plln = 100;  //Set PLLN to 100
	// 00: PLLP = 2
	// 01: PLLP = 4
	// 10: PLLP = 6
	// 11: PLLP = 8
	unsigned pllp = PLLP_DIV_2;  // Set PLLP to 2

	setup_pll(wait_states, pll_src, pllm, plln, pllp);
}

// Set up Phase Locked Loop to get the system clock f_HCLK = 80 MHz
void setup_pll_80mhz () {
	// For f_HCLK = 80 MHz at 3.3 V the number of wait states ust be set to 2
	unsigned wait_states = FLASH_ACR_LATENCY_2WS;
	// HSE source on STM32F411E-DISCO is 8 MHz
	unsigned pll_src = RCC_PLLCFGR_PLLSRC_HSE;
	unsigned pllm = 4;  // Set PLLM to 4
	unsigned plln = 80;  //Set PLLN to 80
	// 00: PLLP = 2
	// 01: PLLP = 4
	// 10: PLLP = 6
	// 11: PLLP = 8
	unsigned pllp = PLLP_DIV_2;  // Set PLLP to 2

	setup_pll(wait_states, pll_src, pllm, plln, pllp);
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
