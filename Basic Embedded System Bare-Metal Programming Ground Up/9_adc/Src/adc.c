#include "adc.h"
#define IOPAEN			(1U<<2)
#define ADC1EN			(1U<<9)
#define ADC_CH1			(1U<<0)
#define ADC_SEQ_LEN_1	0x00
#define CR2_ADON		(1U<<0)
#define CR2_SWSTART		(1U<<22)
#define SR_EOC			(1U<<1)
void pa1_adc_init(void)
{
	/* Enable clock access to GPIOA */
	RCC->APB2ENR |= IOPAEN;
	/* Set the mode of PA1 to analog */
	GPIOA->CRL &=~(1U<<4);
	GPIOA->CRL &=~(1U<<5);
	GPIOA->CRL &=~(1U<<6);
	GPIOA->CRL &=~(1U<<7);
	/*-----------Configure the ADC module-----------*/
	/* Enable clock access to ADC */
	RCC->APB2ENR |= ADC1EN;
	/* Conversion sequence start */
	ADC1->SQR3 = ADC_CH1;
	/* Conversion sequence length */
	ADC1->SQR1 = ADC_SEQ_LEN_1;

	ADC1->CR2 |= (1U<<17);
	ADC1->CR2 |= (1U<<18);
	ADC1->CR2 |= (1U<<19);
	ADC1->CR2 |= (1U<<20);

	/* Enable ADC module */
	ADC1->CR2 |= CR2_ADON;
}
void start_conversion(void)
{
	ADC1->CR2 |= CR2_SWSTART;
}
uint32_t adc_read(void)
{
	/* Wait for conversion to be completed */
	while(!(ADC1->SR & SR_EOC));
	/* Read converted result */
	return (ADC1->DR);
}
