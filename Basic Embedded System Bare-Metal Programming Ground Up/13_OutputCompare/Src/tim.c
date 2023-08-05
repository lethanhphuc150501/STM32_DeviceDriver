#include "stm32f1xx.h"

#define TIM2EN			(1U<<0)
#define CR1_CEN			(1U<<0)
#define OC_TOGGLE		((1U<<4) | (1U<<5))
#define CCER_CC1E		(1U<<0)
#define IOPAEN			(1U<<2)

void TIM2_1Hz_init(void)
{
	/* Enable clock access to TIM2 */
	RCC->APB1ENR |= TIM2EN;

	/* Set prescaler value */
	TIM2->PSC = 800 - 1; 				// 	8 000 000 / 800 = 10 000

	/* Set auto-reload value */
	TIM2->ARR = 10000 - 1;				// 10 000 / 10 000 = 1

	/* Clear counter */
	TIM2->CNT = 0;

	/* Enable timer */
	TIM2->CR1 |= CR1_CEN;
}

void TIM2_PA0_output_compare(void)
{
	/* Enable clock access to GPIOA */
	RCC->APB2ENR |= IOPAEN;

	/* Set PA0 mode to alternate function */
	GPIOA->CRL |= (1U<<1);
	GPIOA->CRL |= (1U<<0);
	GPIOA->CRL &=~(1U<<2);
	GPIOA->CRL |= (1U<<3);

	/* Enable clock access to TIM2 */
	RCC->APB1ENR |= TIM2EN;

	/* Set prescaler value */
	TIM2->PSC = 800 - 1; 				// 	8 000 000 / 800 = 10 000

	/* Set auto-reload value */
	TIM2->ARR = 10000 - 1;				// 10 000 / 10 000 = 1

	/* Set output compare toggle mode */
	TIM2->CCMR1 = OC_TOGGLE;

	/* Enable TIM2_CH1 in compare mode */
	TIM2->CCER |= CCER_CC1E;

	/* Clear counter */
	TIM2->CNT = 0;

	/* Enable timer */
	TIM2->CR1 |= CR1_CEN;
}

