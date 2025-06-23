# Microcontrollers Programming Laoratory

This directory aims to provide a reference for the register-level programming of
the STM32 microcontrollers.

## Structure

Each subdirectory contains a source code, necessary libraries, and a `Makefile`
for the STM development board target compilation and flashing:

```sh
STM32F407G-DISC1
├── Makefile
├── drv         # ST-provided files
├── inc         # header files
└── src         # Source files
```

To build the binary run `make`. To flash the target board run `make flash`.

## Laboratory work plan

The main tasks to be completed throughout the laboratory cover implementing:

1. A function that configures the clock source to:

- High-Speed External (HSE) clock,
- High-Speed Internal (HSI) clock,
- Phase-Locked Loop (PLL) to the frequency selected by the user.

2. A custom delay function which uses an interrupt and the Systic timer.

3. A function which:

- configures the pins connected to the LEDs as outputs and buttons as input;
- toggles the active LED upon the user button press.

4. Sets the PWM modulation for the LEDs. The user button press toggles the PWM
duty cycle from 0 to 100% in the increments of 20%.

5. Reading data from the on-board accelerometer.

6. Driving the LEDs intencities based on the board inclination.

## Makefile

```sh
gcc -- help
  -c                       Compile and assemble, but do not link.
```
