#include <stm32f1xx.h>
#include <stm32f103xb.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"

#define IOPAEN		(1U<<2)
#define PIN5		(1U<<5)
#define LED_PIN		PIN5

char key;

int main(void)
{
	/* Enable clock access to GPIOA */
	RCC->APB2ENR |= IOPAEN;

	/* Set PA5 as output pin */
	GPIOA->CRL |= (1U<<21);
	GPIOA->CRL &=~(1U<<20);
	GPIOA->CRL &=~(1U<<22);
	GPIOA->CRL &=~(1U<<23);

	UART2_RX_interrupt_init();

	while(1)
	{

	}
}

static void uart_callback()
{
	key = USART2->DR;
	if(key == '1')
	{
		GPIOA->ODR |= LED_PIN;
	}
	else
	{
		GPIOA->ODR &=~LED_PIN;
	}
}

void USART2_IRQHandler()
{
	if(USART2->SR & SR_RXNE)
	{
		// Do something
		uart_callback();
	}
}
