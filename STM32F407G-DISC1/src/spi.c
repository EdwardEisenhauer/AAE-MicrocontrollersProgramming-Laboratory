#include "spi.h"

// Set up Serial Peripheral Interface 1 in the master mode.
void setup_spi (void) {
	// Enable clock for the SPI peripheral.
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  // Enable clock for the GPIO ports A and E.
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN |
                  RCC_AHB1ENR_GPIOEEN;

	// Alternate function modes.
  GPIOA->MODER &= ~GPIO_MODER_MODER5;
  GPIOA->MODER &= ~GPIO_MODER_MODER6;
  GPIOA->MODER &= ~GPIO_MODER_MODER7;

  GPIOA->MODER |= GPIO_MODER_MODER5_1;
  GPIOA->MODER |= GPIO_MODER_MODER6_1;
  GPIOA->MODER |= GPIO_MODER_MODER7_1;

	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL6;
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL7;

	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_2 | GPIO_AFRL_AFSEL5_0; // Set pin PA5 in the SPI1_SCK mode.
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL6_2 | GPIO_AFRL_AFSEL6_0; // Set pin PA6 in the SPI1_MISO mode.
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL7_2 | GPIO_AFRL_AFSEL7_0; // Set pin PA7 in the SPI1_MOSI mode.

  GPIOE->MODER &= ~(3 << (2 * 3));  // Clear mode bits
	GPIOE->MODER |=  (1 << (2 * 3));  // Set to output mode
  GPIOE->OTYPER &= ~(1 << 3);       // Push-pull
  GPIOE->OSPEEDR |= (3 << (2 * 3)); // High speed
	GPIOE->PUPDR &= ~(3 << (2 * 3));  // No pull-up/down

  SPI1->CR1 = SPI_CR1_CPHA |	// Clock phase: The second clock transition is the first data capture edge.
			        SPI_CR1_CPOL |  // Clock polarity: CK to 1 when idle
			        SPI_CR1_MSTR |  // Master configuration
				      SPI_CR1_BR_1 | SPI_CR1_BR_0 | // Baud rate control --  f_PCLK/16
				      SPI_CR1_LSBFIRST | // Frame format: LSB transmitted first
				      SPI_CR1_SSM | // Software slave management enabled
				      SPI_CR1_SSI;  // The value of this bit is forced onto the NSS pin and the IO value of the NSS pin is ignored.
              // SPI_CR1_RXONLY | // Full duplex (Transmit and receive)
							// 8-bit data frame format is selected for transmission/reception
	SPI1->CR1 |= SPI_CR1_SPE; // SPI enabled
  SPI1->CR2 = 0;

}

void spi_write (uint8_t data) {
  GPIOE->ODR &= ~(1 << 3);
  while (!(SPI1->SR & SPI_SR_TXE)); // Wait for the Transmitter buffer to be empty.
  SPI1->DR = data;
  while (!(SPI1->SR & SPI_SR_RXNE));
  volatile uint8_t dummy = SPI1->DR;  // Read received data to clear RXNE
  GPIOE->ODR |= (1 << 3);
}
