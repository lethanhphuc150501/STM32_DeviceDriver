#ifndef UART_H_
#define UART_H_

#include "stm32f1xx.h"
#include "stm32f103xb.h"

#include <stdint.h>

#define SR_RXNE			(1U<<5)
#define ISR_TCIF7		(1U<<25)
#define IFCR_TCIF7		(1U<<25)

char uart2_read(void);
void UART2_TX_init(void);
void UART2_RX_TX_init(void);
void UART2_RX_interrupt_init(void);
void DMA1_CH7_init(uint32_t src, uint32_t dst, uint32_t len);

#endif /* UART_H_ */
