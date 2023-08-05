#include "adc_dma.h"

#define IOPAEN			(1U<<2)
#define ADC1EN			(1U<<9)
#define DMA1EN			(1U<<0)

#define CR1_SCAN		(1U<<8)
#define CR2_CONT		(1U<<1)
#define CR2_DMA			(1U<<8)
#define CR2_ADON		(1U<<0)
#define CR2_SWSTART		(1U<<26)

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
	GPIOA->CRL &=~(1U<<0);
	GPIOA->CRL &=~(1U<<1);
	GPIOA->CRL &=~(1U<<2);
	GPIOA->CRL &=~(1U<<3);
	GPIOA->CRL &=~(1U<<4);
	GPIOA->CRL &=~(1U<<5);
	GPIOA->CRL &=~(1U<<6);
	GPIOA->CRL &=~(1U<<7);

	/*---------------ADC Configuration----------------*/
	/* Enable clock access to ADC1 */
	RCC->APB2ENR |= ADC1EN;

	/* Set sequence length: 2 conversions */
	ADC1->SQR1 |= (1U<<20);
	ADC1->SQR1 &=~(1U<<21);
	ADC1->SQR1 &=~(1U<<22);
	ADC1->SQR1 &=~(1U<<23);

	/* Set sequence */
	ADC1->SQR3 = (0U<<0) | (1U<<5);

	/* Enable scan mode */
	ADC1->CR1 = CR1_SCAN;

	/* Select to use DMA */
	ADC1->CR2 |= CR2_CONT;
	ADC1->CR2 |= CR2_DMA;

	/*---------------DMA Configuration----------------*/
	/* Enable clock access to DMA1 */
	RCC->AHBENR |= DMA1EN;

	/* Disable DMA1 Channel 1 */
	DMA1_Channel1->CCR &=~CCR_EN;

	/* Wait till DMA1 Channel 1 is disabled */
	while(DMA1_Channel1->CCR & CCR_EN);

	/* Enable Circular mode */
	DMA1_Channel1->CCR |= CCR_CIRC;

	/* Set memory transfer size: 16-bits */
	DMA1_Channel1->CCR |= (1U<<10);
	DMA1_Channel1->CCR &=~(1U<<11);

	/* Set peripheral transfer size */
	DMA1_Channel1->CCR |= (1U<<8);
	DMA1_Channel1->CCR &=~(1U<<9);

	/* Enable memory increment */
	DMA1_Channel1->CCR |= CCR_MINC;

	/* Set peripheral address */
	DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));

	/* Set memory address */
	DMA1_Channel1->CMAR = (uint32_t)(&adc_raw_data);

	/* Set number of transfer */
	DMA1_Channel1->CNDTR = (uint16_t)(NUM_OF_CHANNELS);

	/* Enable DMA1 Channel 1 */
	DMA1_Channel1->CCR |= CCR_EN;

	/*---------------Enable ADC Module----------------*/
	/* Enable ADC */
	ADC1->CR2 |= CR2_ADON;

	/* Start ADC */
	ADC1->CR2 |= CR2_SWSTART;
}
