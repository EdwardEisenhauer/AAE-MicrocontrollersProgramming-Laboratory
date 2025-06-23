#include "demo.h"

// Set up the user button on STM32F407G-DISC1
static void setup_button () {
    // The B1 USER pushbutton is connected to the PA0 pin.
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable the GPIO port A clock
    GPIOA->MODER &= ~GPIO_MODER_MODER0;  // Set pin 0 in input mode
}

// Setup Microcontroller Clock Output 2 for the clocking frequency monitoring.
static void setup_mco2 () {
    // | MCO  | Port |
    // | ---- | ---- |
    // | MCO1 | PA8  |
    // | MCO2 | PC9  |

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable the GPIO port C clock.

    GPIOC->MODER &= ~GPIO_MODER_MODER9; // Clear the PC9 MODER.
    GPIOC->MODER |= GPIO_MODER_MODER9_1; // Set port PC9 in alternate function mode.
    GPIOC->AFR[1] &= ~GPIO_AFRH_AFSEL9; // AF0.
    GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED9; // High speed.

    RCC->CFGR &= ~RCC_CFGR_MCO2PRE; // Set MCO2 prescaler to no division.
    RCC->CFGR &= ~RCC_CFGR_MCO2; // Set MCO2 to SYSCLK.
}

void demo (void) {
	setup_leds(false);
  setup_button();
  setup_systick(SYSTICK_TICKS);
  setup_mco2();

  setup_hsi();
  GPIOD->BSRR = LED_GREEN_ON;
  wait_ms(DELAY_MS);
  while (! (GPIOA->IDR & GPIO_IDR_ID0) );
  
  setup_hse();
  GPIOD->BSRR = LED_ORANGE_ON;
  wait_ms(DELAY_MS);
  while (! (GPIOA->IDR & GPIO_IDR_ID0) );

  while (1) {
    turn_off_leds();
    wait_ms(DELAY_MS);
    turn_on_leds();
    wait_ms(DELAY_MS);
  }
}