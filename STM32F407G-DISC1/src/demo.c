#include "demo.h"

// Set up the user button on STM32F407G-DISC1
static void setup_button () {
    // The B1 USER pushbutton is connected to the PA0 pin.
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable the GPIO port A clock
    GPIOA->MODER &= ~GPIO_MODER_MODER0;  // Set pin 0 in input mode
}

static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}

void demo (void) {

	setup_leds(false);
  setup_button();

  while (1) {
    turn_on_leds();
    spin(999999);
    turn_off_leds();
    spin(999999);
  }
}