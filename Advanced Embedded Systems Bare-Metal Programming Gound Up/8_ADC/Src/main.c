#include "stm32f1xx.h"
#include "dma.h"
#include "uart.h"
#include "adc_dma.h"

extern uint16_t adc_raw_data[NUM_OF_CHANNELS];

int main(void)
{
	adc_dma_init();

	while(1)
	{

	}
}
