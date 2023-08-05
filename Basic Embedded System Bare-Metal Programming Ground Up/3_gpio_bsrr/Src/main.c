#include <stm32f1xx.h>
#include <stm32f103xb.h>

#define IOPAEN		(1U<<2)
#define PIN5		(1U<<5)
#define LED_PIN		PIN5

int main(void)
{
	RCC->APB2ENR |= IOPAEN;
	GPIOA->CRL |= (1U<<21);
	GPIOA->CRL &=~(1U<<20);
	GPIOA->CRL &=~(1U<<22);
	GPIOA->CRL &=~(1U<<23);
	while(1)
	{
		GPIOA->BSRR = LED_PIN;
		for(int i=0; i<100000; i++);
		GPIOA->BSRR = (1U<<21);
		for(int i=0; i<100000; i++);
	}
}
