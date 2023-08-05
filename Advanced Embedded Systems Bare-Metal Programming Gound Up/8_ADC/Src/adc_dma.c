#include "adc_dma.h"

#define IOPAEN			(1U<<2)
#define ADC1EN			(1U<<9)
#define DMA1EN			(1U<<0)

#define CR1_SCAN		(1U<<8)
#define CR2_CONT		(1U<<1)
#define CR2_DMA			(1U<<8)
#define CR2_ADON		(1U<<0)
#define CR2_SWSTART		(1U<<30)

#define CCR_EN			(1U<<0)
#define CCR_CIRC		(1U<<5)
#define CCR_MINC		(1U<<7)

uint16_t adc_raw_data[NUM_OF_CHANNELS];

void adc_dma_init(void)
{
	/*---------------GPIO Configuration---------------*/
	/* Enable clock access to GPIOA */
	RCC->APB2ENR |= IOPAEN;

	/* Set PA0 & PA1 to Input Analog mode */
	GPIOA->CRL &=~(1U<<4);
	GPIOA->CRL &=~(1U<<5);
	GPIOA->CRL &=~(1U<<6);
	GPIOA->CRL &=~(1U<<7);

	/*---------------ADC Configuration----------------*/
	/* Enable clock access to ADC1 */
	RCC->APB2ENR |= ADC1EN;

	/* Set sequence length: 2 conversions */
	ADC1->SQR1 = 0x00;

	/* Set sequence */
	ADC1->SQR3 = (1U<<0);

	/* Select to use DMA */
	ADC1->CR2 |= CR2_CONT;

	/*---------------Enable ADC Module----------------*/
	/* Enable ADC */
	ADC1->CR2 |= CR2_ADON;

	/* Start ADC */
	ADC1->CR2 |= CR2_SWSTART;
}
