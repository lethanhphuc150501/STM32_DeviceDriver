#include <stm32f1xx.h>
#include <stm32f103xb.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "adc.h"

uint32_t sensor_value;

int main(void)
{
	UART2_TX_init();
	pa1_adc_init();

	while(1)
	{
		start_conversion();
		sensor_value = adc_read();
		printf("Sensor value: %d \n\r", (int)sensor_value);
	}
}
