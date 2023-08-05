#include <stm32f1xx.h>
#include <stm32f103xb.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

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

	UART2_TX_init();
	TIM2_1Hz_interrupt_init();

	while(1)
	{

	}
}

static void TIM2_Callback(void)
{
	printf("A second passed! \n\r");
	GPIOA->ODR ^= LED_PIN;
}

void TIM2_IRQHandler(void)
{
	if(TIM2->SR & SR_UIF)
	{
		/* Clear UIF */
		TIM2->SR &=~SR_UIF;
		// Do something
		TIM2_Callback();
	}
}
