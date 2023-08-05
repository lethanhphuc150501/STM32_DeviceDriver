#include "dma.h"
#include "stm32f1xx.h"

#define DMA1EN			(1U<<0)

#define CCR_EN			(1U<<0)
#define CCR_MINC		(1U<<7)
#define CCR_PINC		(1U<<6)
#define CCR_DIR_FR_MEM	(1U<<4)
#define CCR_MEM2MEM		(1U<<14)
#define CCR_TCIE		(1U<<1)
#define CCR_TEIE		(1U<<3)

void dma1_mem2mem_config(void)
{
	/* Enable clock access to DMA1 module */
	RCC->AHBENR |= DMA1EN;

	/* Disable DMA channel */
	DMA1_Channel1->CCR = 0;

	/* Wait until stream is disabled */
	while(DMA1_Channel1->CCR & CCR_EN);

	/*---------------Configure DMA parameters---------------*/
	/* Set memory size to 16-bits */
	DMA1_Channel1->CCR |= (1U<<10);
	DMA1_Channel1->CCR &=~(1U<<11);

	/* Set peripheral size to 16-bits */
	DMA1_Channel1->CCR |= (1U<<8);
	DMA1_Channel1->CCR &=~(1U<<9);

	/* Enable memory increment */
	DMA1_Channel1->CCR |= CCR_MINC;

	/* Enable peripheral increment */
	DMA1_Channel1->CCR |= CCR_PINC;

	/* Configure transfer direction: memory to memory */
	DMA1_Channel1->CCR |= CCR_DIR_FR_MEM;
	DMA1_Channel1->CCR |= CCR_MEM2MEM;

	/* Enable transfer complete interrupt */
	DMA1_Channel1->CCR |= CCR_TCIE;

	/* Enable transfer error interrupt */
	DMA1_Channel1->CCR |= CCR_TEIE;

	/* Enable DMA interrupt in NVIC */
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void dma_transfer_start(uint32_t src_buff, uint32_t dest_buff, uint32_t len)
{
	/* Set peripheral address */
	DMA1_Channel1->CPAR = dest_buff;

	/* Set memory address */
	DMA1_Channel1->CMAR = src_buff;

	/* Set transfer length */
	DMA1_Channel1->CNDTR = len;

	/* Enable DMA Channel */
	DMA1_Channel1->CCR |= CCR_EN;
}
