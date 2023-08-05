#include "adc.h"

uint32_t sensor_value;

int main(void)
{
	pa1_adc_init();
	while(1)
	{
		start_conversion();
		sensor_value = adc_read();
	}
}
