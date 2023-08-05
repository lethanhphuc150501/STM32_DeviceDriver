#include "stm32f1xx.h"
#include "dma.h"
#include "uart.h"

#define BUFFER_SIZE			5

uint16_t sensor_data_arr[BUFFER_SIZE] = {892, 731, 1234, 90, 23};
uint16_t temp_data_arr[BUFFER_SIZE];

volatile uint8_t g_transfer_cmplt = 0;

int main(void)
{
	UART2_TX_init();

	dma1_mem2mem_config();
	dma_transfer_start((uint32_t)sensor_data_arr, (uint32_t)temp_data_arr, BUFFER_SIZE);

	/* Wait until transfer complete */
	while(!g_transfer_cmplt);

	for(int i = 0; i < BUFFER_SIZE; i++)
	{
		printf("Temp buffer[%d]: %d\r\n", i, temp_data_arr[i]);
	}

	g_transfer_cmplt = 0;

	while(1)
	{

	}
}

void DMA1_Channel1_IRQHandler(void)
{
	/* Check if transfer complete interrupt occurred */
	if(DMA1->ISR & ISR_TCIF1)
	{
		g_transfer_cmplt = 1;

		/* Clear flag */
		DMA1->IFCR |= IFCR_CTCIF1;
	}

	/* Check if transfer error interrupt occurred */
	if(DMA1->ISR & ISR_TEIF1)
	{
		// Do something

		/* Clear flag */
		DMA1->IFCR |= IFCR_CTEIF1;
	}
}
