#ifndef LED_H_
#define LED_H_

#include "main.h"

#define LED_GREEN_ON  GPIO_BSRR_BS12
#define LED_ORANGE_ON GPIO_BSRR_BS13
#define LED_RED_ON    GPIO_BSRR_BS14
#define LED_BLUE_ON   GPIO_BSRR_BS15

#define LED_GREEN_OFF  GPIO_BSRR_BR12
#define LED_ORANGE_OFF GPIO_BSRR_BR13
#define LED_RED_OFF    GPIO_BSRR_BR14
#define LED_BLUE_OFF   GPIO_BSRR_BR15

typedef enum {
    LD4 = 0,
    LD3 = 1,
	LD5 = 2,
	LD6 = 3,
	LED_ORANGE = LD3,
	LED_GREEN = LD4,
	LED_RED = LD5,
	LED_BLUE = LD6,
	LED_UP = LD3,
	LED_LEFT = LD4,
	LED_RIGHT = LD5,
	LED_DOWN = LD6
} LED;

void setup_leds (bool);
void enable_pwm_for_leds ();
void set_pwm (LED, int);
void turn_on_led (LED);
void turn_off_led (LED);
void turn_on_leds ();
void turn_off_leds ();

#endif /* LED_H_ */
