#include "exti.h"

#define IOPCEN		(1U<<4)
#define AFIOEN		(1U<<0)


void PC13_exti_init(void)
{
	/* Disable global interrupt */
	__disable_irq();

	/* Enable clock access for GPIOC */
	RCC->APB2ENR |= IOPCEN;

	/* Configure PC13 as input */
	GPIOC->CRH &=~(1U<<20);
	GPIOC->CRH &=~(1U<<21);
	GPIOC->CRH &=~(1U<<22);
	GPIOC->CRH |= (1U<<23);

	/* Enable clock access for AFIO */
	RCC->APB2ENR |= AFIOEN;

	/* Select PORTC for EXTI13 */
	AFIO->EXTICR[3] |= (1U<<5);

	/* Unmask EXTI13 */
	EXTI->IMR |= (1U<<13);

	/* Select falling edge trigger */
	EXTI->FTSR |= (1U<<13);

	/* Enable EXTI13 line in NVIC */
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* Enable global interrupt */
	__enable_irq();
}
