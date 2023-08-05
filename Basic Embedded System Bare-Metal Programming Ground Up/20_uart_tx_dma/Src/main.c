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
	char message[31] = "Hello from Stm32 DMA transfer\n\r";

	/* Enable clock access to GPIOA */
	RCC->APB2ENR |= IOPAEN;

	/* Set PA5 as output pin */
	GPIOA->CRL |= (1U<<21);
	GPIOA->CRL &=~(1U<<20);
	GPIOA->CRL &=~(1U<<22);
	GPIOA->CRL &=~(1U<<23);

	UART2_TX_init();
	DMA1_CH7_init((uint32_t) message, (uint32_t) &USART2->DR, 31);

	while(1)
	{

	}
}

static void dma_callback(void)
{
	GPIOA->ODR |= LED_PIN;
}

void DMA1_Channel7_IRQHandler(void)
{
	/* Check for transfer complete interrupt */
	if(DMA1->ISR & ISR_TCIF7)
	{
		/* Clear flag */
		DMA1->IFCR |= IFCR_TCIF7;
		// Do something
		dma_callback();
	}
}
