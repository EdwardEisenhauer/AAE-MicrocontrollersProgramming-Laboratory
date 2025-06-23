#include "led.h"

extern uint32_t core_clock;

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

void enable_pwm_for_leds () {
  // APB - AMBA Peripheral Bus
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	// The counter clock frequency CK_CNT is equal to f_CK_PSC / (PSC[15:0] + 1).
	TIM4->PSC = ( core_clock / 1000000 ) - 1;
	// Timer 4 Auto-Reload Register.
	TIM4->ARR = 1000 - 1;

	// Timer 4 Capture/Compare Mode Register 1.
	TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 |
			           TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
	TIM4->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 |
			           TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;
	// Output compare preload enable
	TIM4->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;
	TIM4->CCMR2 |= TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE;

	// Timer 4 Capture/Compare Enable Register.
	TIM4->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;
	// Auto-reload preload enable.
	TIM4->CR1 |= TIM_CR1_ARPE;
	TIM4->CR1 |= TIM_CR1_CEN;  // Enable Timer 4
}

void set_pwm (LED led, int duty_cycle) {
	if ( duty_cycle >= 100 ) duty_cycle = 100;

	unsigned t_on_ticks = (TIM4->ARR * duty_cycle) / 100;

	switch ( led ) {
		case LD3:
			TIM4->CCR2 = t_on_ticks;
			break;
		case LD4:
			TIM4->CCR1 = t_on_ticks;
			break;
		case LD5:
			TIM4->CCR3 = t_on_ticks;
			break;
		case LD6:
			TIM4->CCR4 = t_on_ticks;
			break;
	}
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
