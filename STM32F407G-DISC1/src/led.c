#include "led.h"

// Set up user LEDs on STM32F407G-DISC1.
// If pwm is set to true, the LEDs will be configured to be driven by PWM.
void setup_leds (bool pwm) {
  // There are four user LEDs:
  //
  // | Name | Colour | Port |
  // | ---- | ------ | ---- |
  // | LD3  | orange | PD13 |
  // | LD4  | green  | PD12 |
  // | LD5  | red    | PD14 |
  // | LD6  | blue   | PD15 |

  // AMBA - Advanced Microcontroller Bus Architecture
  // AHB - AMBA High-performance Bus
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;  // Enable the GPIO port D clock

  // Clear mode bits for pins 12, 13, 14, and 15.
  GPIOD->MODER &= ~(GPIO_MODER_MODER12 |
                    GPIO_MODER_MODER13 |
                    GPIO_MODER_MODER14 |
                    GPIO_MODER_MODER15);
  
  if (pwm) {
    // Set pins 12, 13, 14, and 15 in alternate function mode
    GPIOD->MODER |= GPIO_MODER_MODER13_1 |
                    GPIO_MODER_MODER12_1 |
                    GPIO_MODER_MODER14_1 |
                    GPIO_MODER_MODER15_1;

    // AFR - Alternate Function Register
    GPIOD->AFR[1] &= ~GPIO_AFRH_AFSEL12;
    GPIOD->AFR[1] &= ~GPIO_AFRH_AFSEL13;
    GPIOD->AFR[1] &= ~GPIO_AFRH_AFSEL14;
    GPIOD->AFR[1] &= ~GPIO_AFRH_AFSEL15;

    GPIOD->AFR[1] |= GPIO_AFRH_AFSEL12_1;
    GPIOD->AFR[1] |= GPIO_AFRH_AFSEL13_1;
    GPIOD->AFR[1] |= GPIO_AFRH_AFSEL14_1;
    GPIOD->AFR[1] |= GPIO_AFRH_AFSEL15_1;
  } else {
    // Set pins 12, 13, 14, and 15 in general purpose output mode
    GPIOD->MODER |= GPIO_MODER_MODER12_0;
    GPIOD->MODER |= GPIO_MODER_MODER13_0;
    GPIOD->MODER |= GPIO_MODER_MODER14_0;
    GPIOD->MODER |= GPIO_MODER_MODER15_0;
  
    GPIOD->OTYPER &= ~GPIO_OTYPER_OT12;
    GPIOD->OTYPER &= ~GPIO_OTYPER_OT13;
    GPIOD->OTYPER &= ~GPIO_OTYPER_OT14;
    GPIOD->OTYPER &= ~GPIO_OTYPER_OT15;
  
  }

  GPIOD->OSPEEDR |= GPIO_OSPEEDR_OSPEED15;
  GPIOD->PUPDR &= ~GPIO_PUPDR_PUPD15;
}

void turn_on_led (LED led) {
	GPIOD->BSRR = (GPIO_BSRR_BS12 << led);
}

void turn_off_led (LED led) {
	GPIOD->BSRR = (GPIO_BSRR_BR12 << led);
}

// Turn on all the user LEDs
void turn_on_leds () {
    GPIOD->BSRR = LED_GREEN_ON  |
    			  LED_ORANGE_ON |
				  LED_RED_ON    |
				  LED_BLUE_ON;
}

// Turn off all the user LEDs
void turn_off_leds () {
    GPIOD->BSRR = LED_GREEN_OFF  |
			  	  LED_ORANGE_OFF |
				  LED_RED_OFF    |
				  LED_BLUE_OFF;
}
