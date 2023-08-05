#include "stm32f1xx.h"

#define TIM2EN			(1U<<0)
#define CR1_CEN				(1U<<0)

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
