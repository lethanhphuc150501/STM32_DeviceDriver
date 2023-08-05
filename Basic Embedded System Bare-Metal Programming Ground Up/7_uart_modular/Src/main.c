#include <stm32f1xx.h>
#include <stm32f103xb.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"

int main(void)
{
	UART2_TX_init();

	while(1)
	{
		printf("Hello world! \n\r");
	}
}
