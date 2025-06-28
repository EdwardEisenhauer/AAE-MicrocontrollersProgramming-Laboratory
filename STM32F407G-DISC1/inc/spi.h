#ifndef SPI_H_
#define SPI_H_

#include "main.h"

void spi_cs_low();
void spi_cs_high();
uint8_t spi_write (uint8_t);
uint8_t spi_read (void);
uint8_t spi_read_reg(uint8_t);
void spi_read_multi(uint8_t, uint8_t*, int);
void setup_spi (void);

#endif /* SPI_H_ */
