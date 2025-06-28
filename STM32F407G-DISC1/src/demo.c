#include "demo.h"

extern uint32_t core_clock;

// Setup Microcontroller Clock Output 2 for the clocking frequency monitoring.
// We pick the PC9 pin, as it is located on the outer side of the Discovery
// board header.
static void setup_mco2 (MCO_Division div) {
  // | MCO  | Port |
  // | ---- | ---- |
  // | MCO1 | PA8  |
  // | MCO2 | PC9  |

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable the GPIO port C clock

  GPIOC->MODER   &= ~GPIO_MODER_MODER9;    // Clear the PC9 MODER
  GPIOC->MODER   |=  GPIO_MODER_MODER9_1;  // Set port PC9 in alternate function mode
  GPIOC->AFR[1]  &= ~GPIO_AFRH_AFSEL9;     // AF0 (system)
  GPIOC->OSPEEDR |=  GPIO_OSPEEDR_OSPEED9; // High speed

  RCC->CFGR &= ~RCC_CFGR_MCO2PRE; // Clear the MCO2 prescaler
  RCC->CFGR |= (div << RCC_CFGR_MCO2PRE_Pos); // Set the MCO2 prescaler to div
  RCC->CFGR &= ~RCC_CFGR_MCO2;    // Set MCO2 to SYSCLK
}

// Toggle between user LEDs upon the user button click.
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

// Demonastrate the laboratory tasks.
//
// The frequency of the SYSCLK may be oserviad via the PC9 pin.
void demo (void) {
	setup_leds(false);            // Setup LEDs' GPIOs in the output mode
  setup_button();               // Setup User Button's GPIO in the input mode
  setup_systick(SYSTICK_TICKS); // 
  // Since the maximum f_CLK is 168 MHz and the maximum f_MCO2 is 100 MHz,
  // we must set the MCO_Division to 2.
  setup_mco2(MCO_DIV_2);       // Setup PC9 to output the SYSCLK/2

  setup_hsi();
  GPIOD->BSRR = LED_GREEN_ON;
  wait_for_button_press(DEBOUNCE_DELAY_MS);
  
  setup_hse();
  GPIOD->BSRR = LED_ORANGE_ON;
  wait_for_button_press(DEBOUNCE_DELAY_MS);

  setup_pll_max();
  core_clock = 168000000UL;
  GPIOD->BSRR = LED_RED_ON;
  wait_for_button_press(DEBOUNCE_DELAY_MS);

  setup_pll_80mhz();
  core_clock = 80000000UL;
  GPIOD->BSRR = LED_BLUE_ON;
  wait_for_button_press(DEBOUNCE_DELAY_MS);

  toggle_leds_on_button_press(false);
  wait_for_button_press(DEBOUNCE_DELAY_MS);

  setup_leds(true);
  enable_pwm_for_leds();
  toggle_leds_pwm_on_button_press(false);
  wait_for_button_press(DEBOUNCE_DELAY_MS);
  
  setup_spi();
  setup_lis3dsh();

  int16_t x, y, z;
  uint8_t x_l, x_h, y_l, y_h;

  while (1) {
    wait_ms(10);
    
    spi_cs_low();
    x_l = lis3dsh_read(OUT_X_L);
    x_h = lis3dsh_read(OUT_X_H);
    y_l = lis3dsh_read(OUT_Y_L);
    y_h = lis3dsh_read(OUT_Y_H);
    spi_cs_high();
    
    x = ((x_h << 8) | x_l);
    y = ((y_h << 8) | y_l);

    if ( x > HORIZONTAL_TRESHOLD ) {
      set_pwm(LED_GREEN, x / SCALER );
      set_pwm(LED_RED,             0);
    } else if ( x < -HORIZONTAL_TRESHOLD ) {
      set_pwm(LED_RED, -x / SCALER);
      set_pwm(LED_GREEN,         0);
    } else {
      set_pwm(LED_GREEN, 0);
      set_pwm(LED_RED,   0);
    }

    if ( y > HORIZONTAL_TRESHOLD ) {
      set_pwm(LED_BLUE, y / SCALER);
      set_pwm(LED_ORANGE, 0);
    } else if ( y < -HORIZONTAL_TRESHOLD ) {
      set_pwm(LED_ORANGE, -y / SCALER);  
      set_pwm(LED_BLUE,     0);  
    } else {
      set_pwm(LED_ORANGE, 0);
      set_pwm(LED_BLUE,   0);
    }
  }
}
