#include "lis3dsh.h"

uint8_t lis3dsh_read(uint8_t addr) {
  spi_cs_low();
  spi_write(addr | 0x80); // Set the READ bit
  uint8_t data = spi_write(0x00); // Dummy write to clock out the data
  spi_cs_high();
  return data;
}

void wait_for_lis3dsh (void) {
  set_pwm(LED_BLUE, 100);
  uint8_t id = 0;
  while (1) {
    id = lis3dsh_read(WHO_AM_I);

    if (id == LIS3DSH_ID) {
      set_pwm(LED_GREEN, 100);
      set_pwm(LED_RED, 0);
      return;
    } else {
      set_pwm(LED_RED, 100);
      wait_ms(100);
      set_pwm(LED_RED, 0);
      wait_ms(100);
    }
  }
}

void lis3dsh_write(uint8_t addr, uint8_t data) {
  spi_cs_low();
  spi_write(addr);
  spi_write(data);
  spi_cs_high();
}

void setup_lis3dsh (void) {
  wait_for_lis3dsh();

  lis3dsh_write(
    CTRL_REG4,
    0x60 | // Power up with output data rate 100 Hz.
    0x07   // Continous update, enable X, Y, and Z axes.
  );
}

void lis3dsh_read_xyz (int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buffer[6];
    spi_read_multi(OUT_X_L, buffer, 6);

    *x = (int16_t)((buffer[1] << 8) | buffer[0]);
    *y = (int16_t)((buffer[3] << 8) | buffer[2]);
    *z = (int16_t)((buffer[5] << 8) | buffer[4]);
}
