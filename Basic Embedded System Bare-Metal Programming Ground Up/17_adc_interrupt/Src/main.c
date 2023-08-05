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
	pa1_adc_interrupt_init();
	start_conversion();
	while(1)
	{

	}
}

static void ADC_Callback(void)
{
	sensor_value = ADC1->DR;
	printf("Sensor value: %d \n\r", (int)sensor_value);
}

void ADC1_2_IRQHandler(void)
{
	/* Check for EOC in SR */
	if((ADC1->SR & SR_EOC) != 0)
	{
		// Do something
		ADC_Callback();
	}
}
