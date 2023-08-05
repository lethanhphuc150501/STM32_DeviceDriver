#ifndef UART_H_
#define UART_H_

#include "stm32f1xx.h"
#include "stm32f103xb.h"

#include <stdint.h>

char uart2_read(void);
void UART2_RX_TX_init(void);

#endif /* UART_H_ */
