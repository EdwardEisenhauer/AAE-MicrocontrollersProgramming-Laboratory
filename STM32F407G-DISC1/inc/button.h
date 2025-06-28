#ifndef BUTTON_H_
#define BUTTON_H_

#include "main.h"

#define DEBOUNCE_DELAY_MS 200UL

// Set up the user button on STM32F407G-DISC1
void setup_button ();
void wait_for_button_press (uint32_t);

#endif /* BUTTON_H_ */
