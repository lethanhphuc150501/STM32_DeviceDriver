#include <stm32f1xx.h>
#include <stm32f103xb.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"

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

	PC13_exti_init();
	UART2_TX_init();

	while(1)
	{

	}
}

static void exti_callback(void)
{
	printf("BTN Pressed...\n\r");
	GPIOA->ODR ^= LED_PIN;
}

void EXTI15_10_IRQHandler()
{
	if((EXTI->PR & LINE13) != 0)
	{
		/* Clear PR flag */
		EXTI->PR |= LINE13;
		// Do something
		exti_callback();
	}
}
