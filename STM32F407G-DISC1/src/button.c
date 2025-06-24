#include "button.h"

// Set up the user button on STM32F407G-DISC1
void setup_button () {
  // The B1 USER pushbutton is connected to the PA0 pin.
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable the GPIO port A clock
  GPIOA->MODER &= ~GPIO_MODER_MODER0;  // Set pin 0 in input mode
}

void wait_for_button_press (uint32_t delay_ms) {
  wait_ms(delay_ms);
  while (! (GPIOA->IDR & GPIO_IDR_ID0) );
}
