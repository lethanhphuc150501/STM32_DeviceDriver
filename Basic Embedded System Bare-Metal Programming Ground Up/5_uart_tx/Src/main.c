#include <stm32f1xx.h>
#include <stm32f103xb.h>
#include <stdint.h>

#define IOPAEN			(1U<<2)
#define UART2EN			(1U<<17)

#define CR1_TE			(1U<<3)
#define CR1_UE			(1U<<13)
#define SR_TXE			(1U<<7)

#define SYS_FREQ 		8000000
#define APB1_CLK		SYS_FREQ

#define UART_BAUDRATE	115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
void UART2_TX_init(void);
void uart2_write(int ch);


int main(void)
{
	UART2_TX_init();

	while(1)
	{
		uart2_write('Y');
	}
}

void UART2_TX_init(void)
{
	/*----------Configure UART GPIO pin----------*/
	/* Enable clock access to GPIOA */
	RCC->APB2ENR |= IOPAEN;

	/* Set PA2 mode to alternate function mode */
	GPIOA->CRL |= (1U<<8);
	GPIOA->CRL |= (1U<<9);
	GPIOA->CRL &=~(1U<<10);
	GPIOA->CRL |= (1U<<11);

	/*-----------Configure UART module-----------*/
	/* Enable clock access to UART2 */
	RCC->APB1ENR |= UART2EN;

	/* Configure baud rate */
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/* Configure the transfer direction */
	USART2->CR1 |= CR1_TE;

	/* Enable UART module */
	USART2->CR1 |= CR1_UE;
}

void uart2_write(int ch)
{
	/* Make sure the transmit register is empty */
	while(!(USART2->SR & SR_TXE));

	/* Write to transmit data register */
	USART2->DR = (ch & 0xFF);

}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}
