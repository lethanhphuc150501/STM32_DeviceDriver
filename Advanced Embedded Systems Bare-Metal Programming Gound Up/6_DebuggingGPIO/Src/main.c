#include "stm32f1xx.h"
#include "uart.h"

#define IOPCEN		(1U<<4)
#define PIN13		(1U<<13)
#define BTN_PIN		PIN13
uint8_t button_state;

int main(void)
{
	/* Enable clock access to GPIOC */
	RCC->APB2ENR |= IOPCEN;

	/* Set PC13 as input pin */
	GPIOC->CRH &=~(1U<<20);
	GPIOC->CRH &=~(1U<<21);
	GPIOC->CRH &=~(1U<<22);
	GPIOC->CRH |= (1U<<23);
	while(1)
	{
		/* Check if BTN is pressed */
		if(GPIOC->IDR & BTN_PIN)
		{
			button_state = 0;
		}
		else
		{
			button_state = 1;
		}
		for(int i = 0; i < 90000; i++);
	}
}
