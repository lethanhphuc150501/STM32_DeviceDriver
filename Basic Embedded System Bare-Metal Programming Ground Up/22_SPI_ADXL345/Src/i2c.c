#include "stm32f1xx.h"

#define I2C1EN		(1U<<21)
#define IOPBEN		(1U<<3)

#define CR1_SWRST	(1U<<15)
#define CR1_PE		(1U<<0)
#define CR1_START	(1U<<8)
#define CR1_STOP	(1U<<9)
#define CR1_ACK		(1U<<10)

#define SR1_SB		(1U<<0)
#define SR1_ADDR	(1U<<1)
#define SR1_TXE		(1U<<7)
#define SR1_RXNE	(1U<<6)
#define SR1_BTF		(1U<<2)

#define SR2_BUSY	(1U<<1)

#define I2C_100KHZ					80
#define SD_MODE_MAX_RISE_TIME		17


// PB6 = SCL; PB7 = SDA

void I2C1_init(void)
{
	/* Enable clock access to GPIOB */
	RCC->APB2ENR |= IOPBEN;

	/* Set PA6 & PA7 as alternate function open-drain */
	GPIOB->CRL |= (1U<<24);
	GPIOB->CRL |= (1U<<25);
	GPIOB->CRL |= (1U<<26);
	GPIOB->CRL |= (1U<<27);

	GPIOB->CRL |= (1U<<28);
	GPIOB->CRL |= (1U<<29);
	GPIOB->CRL |= (1U<<30);
	GPIOB->CRL |= (1U<<31);

	/* Enable Pull-up for PB6 & PB7 */
	GPIOB->ODR |= ((1U<<6) | (1U<<7));

	/* Enable clock access to I2C1 */
	RCC->APB1ENR |= I2C1EN;

	/* Enter reset mode */
	I2C1->CR1 |= CR1_SWRST;

	/* Come out of reset mode */
	I2C1->CR1 &=~CR1_SWRST;

	/* Set Peripheral clock frequency */
	I2C1->CR2 = (1U<3);		// 8MHz

	/* Set I2C standard mode, 100kHz clock */
	I2C1->CCR = I2C_100KHZ;

	/* Set rise time */
	I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

	/* Enable I2C1 module */
	I2C1->CR1 |= CR1_PE;
}

void I2C1_byteRead(char saddr, char maddr, char* data)
{
	volatile int tmp;

	/* Wait until bus not busy */
	while(I2C1->SR2 & SR2_BUSY);

	/* Generate start */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_SB));

	/* Transmit slave address + write */
	I2C1->DR = saddr << 1;

	/* Wait until ADDR flag is set */
	while(!(I2C1->SR1 & SR1_ADDR));

	/* Clear ADDR flag */
	tmp = I2C1->SR2;

	/* Send memory address */
	I2C1->DR = maddr;

	/* Wait until transmitter empty */
	while(!(I2C1->SR1 & SR1_TXE));

	/* Generate restart */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_SB));

	/* Transmit slave address + read */
	I2C1->DR = saddr << 1 | 1;

	/* Wait until ADDR flag is set */
	while(!(I2C1->SR1 & SR1_ADDR));

	/* Disable Acknowlegde */
	I2C1->CR1 &=~CR1_ACK;

	/* Clear ADDR flag */
	tmp = I2C1->SR2;

	/* Generate stop after data receive */
	I2C1->CR1 |= CR1_STOP;

	/* Wait until RXNE flag */
	while(!(I2C1->SR1 & SR1_RXNE));

	/* Read data from DR */
	*data++ = I2C1->DR;
}

void I2C1_burstRead(char saddr, char maddr, int n, char* data)
{
	volatile int tmp;
	/* Wait until bus not busy */
	while(I2C1->SR2 & SR2_BUSY);

	/* Generate start */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_SB));

	/* Transmit slave address + write */
	I2C1->DR = saddr << 1;

	/* Wait until ADDR flag is set */
	while(!(I2C1->SR1 & SR1_ADDR));

	/* Clear ADDR flag */
	tmp = I2C1->SR2;

	/* Wait until transmitter empty */
	while(!(I2C1->SR1 & SR1_TXE));

	/* Send memory address */
	I2C1->DR = maddr;

	/* Wait until transmitter empty */
	while(!(I2C1->SR1 & SR1_TXE));

	/* Generate restart */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_SB));

	/* Transmit slave address + read */
	I2C1->DR = saddr << 1 | 1;

	/* Wait until ADDR flag is set */
	while(!(I2C1->SR1 & SR1_ADDR));

	/* Clear ADDR flag */
	tmp = I2C1->SR2;

	/* Enable Acknowlegde */
	I2C1->CR1 |= CR1_ACK;

	while(n > 0U)
	{
		/* If one byte */
		if(n == 1U)
		{
			/* Disable Acknowlegde */
			I2C1->CR1 &=~CR1_ACK;

			/* Generate stop after data receive */
			I2C1->CR1 |= CR1_STOP;

			/* Wait until RXNE flag */
			while(!(I2C1->SR1 & SR1_RXNE));

			/* Read data from DR */
			*data++ = I2C1->DR;
			break;
		}
		else
		{
			/* Wait until RXNE flag */
			while(!(I2C1->SR1 & SR1_RXNE));

			/* Read data from DR */
			*data++ = I2C1->DR;
			n--;
		}
	}
}

void I2C1_burstWrite(char saddr, char maddr, int n, char* data)
{
	volatile int tmp;

	/* Wait until bus not busy */
	while(I2C1->SR2 & SR2_BUSY);

	/* Generate start */
	I2C1->CR1 |= CR1_START;

	/* Wait until start flag is set */
	while(!(I2C1->SR1 & SR1_SB));

	/* Transmit slave address + write */
	I2C1->DR = saddr << 1;

	/* Wait until ADDR flag is set */
	while(!(I2C1->SR1 & SR1_ADDR));

	/* Clear ADDR flag */
	tmp = I2C1->SR2;

	/* Wait until transmitter empty */
	while(!(I2C1->SR1 & SR1_TXE));

	/* Send memory address */
	I2C1->DR = maddr;

	for(int i=0; i<n; i++)
	{
		/* Wait until transmitter empty */
		while(!(I2C1->SR1 & SR1_TXE));

		/* Transmit memory address */
		I2C1->DR = *data++;
	}

	/* Wait until transfer finished */
	while(!(I2C1->SR1 & SR1_BTF));

	/* Generate stop */
	I2C1->CR1 |= CR1_STOP;
}
