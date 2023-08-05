#include "stm32f1xx.h"
#include "uart.h"

int main(void)
{
	UART2_TX_init();
	while(1)
	{
		printf("Hello from main function \n\r");
	}
}
