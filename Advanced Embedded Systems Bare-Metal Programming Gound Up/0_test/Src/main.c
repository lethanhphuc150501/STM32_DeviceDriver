#include "stm32f1xx.h"

#define IOPAEN				(1U<<2)

#define PIN5				(1U<<5)
#define LED_PIN				PIN5

/*
 * (1U<<21)	// Set bit 21 to 1
 * &=~(1U<<20)	// Set bit 20 to 0 */

int main(void)
{
	/* 1. Enable clock access to GPIOA */
	RCC->APB2ENR |= IOPAEN;

	/* 2. Set PA5 as output pin */
	GPIOA->CRL |= (1U<<21);	// Set bit 21 to 1
	GPIOA->CRL &=~(1U<<20);	// Set bit 20 to 0
	GPIOA->CRL &=~(1U<<22);
	GPIOA->CRL &=~(1U<<23);

	while(1)
	{
		GPIOA->ODR ^= LED_PIN;
		for(int i=0; i<100000; i++);
	}
}
