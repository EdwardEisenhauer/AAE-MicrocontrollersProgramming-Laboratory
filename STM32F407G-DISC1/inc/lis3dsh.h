#ifndef LIS3DSH_H_
#define LIS3DSH_H_

#include "main.h"

#define LIS3DSH_ID 0x3F

// Registers
#define OUT_T     0x0C // Temperature output
#define WHO_AM_I  0x0F // Who I am ID
#define CTRL_REG4 0x20 //Control register 4
#define CTRL_REG5 0x24 //Control register 5
#define OUT_X_L   0x28 // Output X Low register
#define OUT_X_H   0x29 // Output X High register
#define OUT_Y_L   0x2A // Output Y Low register
#define OUT_Y_H   0x2B // Output Y High register

// Set up the accelerometer on STM32F407G-DISC1
void wait_for_lis3dsh(void);
void setup_lis3dsh (void);
uint8_t lis3dsh_read(uint8_t);
void lis3dsh_write(uint8_t, uint8_t);
void lis3dsh_read_xyz(int16_t*, int16_t*, int16_t*);

#endif /* LIS3DSH_H_ */
