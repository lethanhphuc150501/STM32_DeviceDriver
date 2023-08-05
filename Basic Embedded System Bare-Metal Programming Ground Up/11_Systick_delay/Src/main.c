#include <stm32f1xx.h>
#include <stm32f103xb.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"

#define IOPAEN		(1U<<2)
#define PIN5		(1U<<5)
#define LED_PIN		PIN5

int main(void)
{
	UART2_TX_init();
	RCC->APB2ENR |= IOPAEN;
	GPIOA->CRL |= (1U<<21);
	GPIOA->CRL &=~(1U<<20);
	GPIOA->CRL &=~(1U<<22);
	GPIOA->CRL &=~(1U<<23);

	while(1)
	{
		printf("A second passed! \n\r");
		GPIOA->ODR ^= LED_PIN;
		systickDelayMs(1000);
	}
}
