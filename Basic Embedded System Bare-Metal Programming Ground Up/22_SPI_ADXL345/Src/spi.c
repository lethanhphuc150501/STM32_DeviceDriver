#include "spi.h"

#define SPI1EN			(1U<<12)
#define IOPAEN			(1U<<2)

#define SR_TXE			(1U<<1)
#define SR_RXNE			(1U<<0)
#define SR_BSY			(1U<<7)

/*
 *	PA4 -> NSS
 * 	PA5 -> SCK
 * 	PA6 -> MISO
 * 	PA7 -> MOSI
 */

void spi_gpio_init(void)
{
	/* Enable clock access to GPIOA */
	RCC->APB2ENR |= IOPAEN;

	/* Set PA4 as Output push-pull */
	GPIOA->CRL |= (1U<<16);
	GPIOA->CRL |= (1U<<17);
	GPIOA->CRL &=~(1U<<18);
	GPIOA->CRL &=~(1U<<19);

	/* Set PA5 as Alternate function push-pull */
	GPIOA->CRL |= (1U<<20);
	GPIOA->CRL |= (1U<<21);
	GPIOA->CRL &=~(1U<<22);
	GPIOA->CRL |= (1U<<23);

	/* Set PA6 as Input Floating */
	GPIOA->CRL &=~(1U<<24);
	GPIOA->CRL &=~(1U<<25);
	GPIOA->CRL |= (1U<<26);
	GPIOA->CRL &=~(1U<<27);

	/* Set PA7 as Alternate function push-pull */
	GPIOA->CRL |= (1U<<28);
	GPIOA->CRL |= (1U<<29);
	GPIOA->CRL &=~(1U<<30);
	GPIOA->CRL |= (1U<<31);
}

void spi_config(void)
{
	/* Enable clock access to SPI1 module */
	RCC->APB2ENR |= SPI1EN;

	/* Set clock to fPCLK/2 */
	SPI1->CR1 &=~(1U<<3);
	SPI1->CR1 &=~(1U<<4);
	SPI1->CR1 &=~(1U<<5);

	/* Set CPOL to 1 and CPHA to 1 */
	SPI1->CR1 |= (1U<<0);
	SPI1->CR1 |= (1U<<1);

	/* Enable full duplex */
	SPI1->CR1 &=~(1U<<10);

	/* Set MSB first */
	SPI1->CR1 &=~(1U<<7);

	/* Set mode to MASTER */
	SPI1->CR1 |= (1U<<2);

	/* Set 8-bit data mode */
	SPI1->CR1 &=~(1U<<11);

	/* Select software slave management by setting SSM=1 and SSI=1 */
	SPI1->CR1 |= (1U<<8);
	SPI1->CR1 |= (1U<<9);

	/* Enable SPI1 module */
	SPI1->CR1 |= (1U<<6);
}

void spi1_transmit(uint8_t *data, uint32_t size)
{
	uint32_t i=0;
	uint8_t temp;
	while(i < size)
	{
		/* Wait until TXE is set */
		while(!(SPI1->SR & SR_TXE));

		/* Write the data to the data register */
		SPI1->DR = data[i];
		i++;
	}
	/* Wait until TXE is set */
	while(!(SPI1->SR & SR_TXE));

	/* Wait for BUSY flag to reset */
	while(SPI1->SR & SR_BSY);

	/* Clear OVR flag */
	temp = SPI1->DR;
	temp = SPI1->SR;
}

void spi1_receive(uint8_t *data, uint32_t size)
{
	while(size)
	{
		/* Send dummy data */
		SPI1->DR = 0;

		/* Wait for RXNE flag to set */
		while(!(SPI1->SR & SR_RXNE));

		/* Read data from data register */
		*data++ = SPI1->DR;
		size--;
	}
}

void cs_enable(void)
{
	GPIOA->ODR &=~(1U<<4);
}

void cs_disable(void)
{
	GPIOA->ODR |= (1U<<4);
}
