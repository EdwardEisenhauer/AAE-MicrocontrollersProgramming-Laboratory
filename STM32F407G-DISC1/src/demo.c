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

// Toggle between user LEDs upon the user button click
void toggle_leds_on_button_press (bool loop) {
  uint8_t counter = 0;
  turn_off_leds();

  while (1) {
    if (GPIOA->IDR & GPIO_IDR_ID0) {
      turn_off_leds();
      GPIOD->BSRR = ( GPIO_BSRR_BS12 << ( counter % 4) );
      counter++;
      while (GPIOA->IDR & GPIO_IDR_ID0);
    }
    if ( !loop && counter == 4) break;
    wait_ms(100);
  }
}

// Toggle between LEDs PWM upon the user button click
void toggle_leds_pwm_on_button_press (bool loop) {
  uint8_t counter = 0;

  while (1) {
    if (GPIOA->IDR & GPIO_IDR_ID0) {
      set_pwm(LD3, 20 * ( counter % 6));
      set_pwm(LD4, 20 * ( counter % 6));
      set_pwm(LD5, 20 * ( counter % 6));
      set_pwm(LD6, 20 * ( counter % 6));
      counter++;
      while (GPIOA->IDR & GPIO_IDR_ID0);
    }
    if ( !loop && counter == 6) break;
    wait_ms(100);
  }
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

  setup_pll_max();
  GPIOD->BSRR = LED_RED_ON;
  wait_ms(DELAY_MS);
  while (! (GPIOA->IDR & GPIO_IDR_ID0) );

  setup_pll_80mhz();
  GPIOD->BSRR = LED_BLUE_ON;
  wait_ms(DELAY_MS);
  while (! (GPIOA->IDR & GPIO_IDR_ID0) );

  toggle_leds_on_button_press(false);
  wait_ms(DELAY_MS);
  while (! (GPIOA->IDR & GPIO_IDR_ID0) );

  setup_leds(true);
  enable_pwm_for_leds();
  toggle_leds_pwm_on_button_press(false);
  wait_ms(DELAY_MS);
  while (! (GPIOA->IDR & GPIO_IDR_ID0) );

  setup_leds(false);
  while (1) {
    turn_off_leds();
    wait_ms(DELAY_MS);
    turn_on_leds();
    wait_ms(DELAY_MS);
  }
}