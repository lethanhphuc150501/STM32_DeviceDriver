#include <stm32f1xx.h>
#include <stm32f103xb.h>

#define IOPAEN		(1U<<2)
#define IOPCEN		(1U<<4)

#define PIN5		(1U<<5)
#define PIN13		(1U<<13)

#define LED_PIN		PIN5
#define BTN_PIN		PIN13

int main(void)
{
	/* Enable clock access to GPIOA & GPIOC */
	RCC->APB2ENR |= IOPAEN;
	RCC->APB2ENR |= IOPCEN;
	/* Set PA5 as output pin */
	GPIOA->CRL |= (1U<<21);
	GPIOA->CRL &=~(1U<<20);
	GPIOA->CRL &=~(1U<<22);
	GPIOA->CRL &=~(1U<<23);
	/* Set PC13 as input pin */
	GPIOC->CRH &=~(1U<<20);
	GPIOC->CRH &=~(1U<<21);
	GPIOC->CRH &=~(1U<<22);
	GPIOC->CRH |= (1U<<23);
	while(1)
	{
		/* Check if BTN is pressed */
		if(GPIOC->IDR & PIN13)
		{
			/* Turn on LED */
			GPIOA->BSRR = LED_PIN;
		}
		else
		{
			/* Turn off LED */
			GPIOA->BSRR = (1U<<21);
		}
	}
}
