#include <stm32f1xx.h>
#include <stm32f103xb.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "adc.h"

uint32_t sensor_value;

int main(void)
{
	pa1_adc_init();
	start_conversion();
	while(1)
	{
		sensor_value = adc_read();
	}
}
