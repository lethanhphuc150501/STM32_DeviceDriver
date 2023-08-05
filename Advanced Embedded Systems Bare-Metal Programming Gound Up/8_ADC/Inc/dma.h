#ifndef DMA_H_
#define DMA_H_

#include "stm32f1xx.h"
#include <stdint.h>

#define ISR_TCIF1			(1U<<1)
#define IFCR_CTCIF1			(1U<<1)
#define ISR_TEIF1			(1U<<3)
#define IFCR_CTEIF1			(1U<<3)

void dma1_mem2mem_config(void);
void dma_transfer_start(uint32_t src_buff, uint32_t dest_buff, uint32_t len);

#endif /* DMA_H_ */
