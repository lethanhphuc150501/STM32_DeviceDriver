#include "uart.h"

#define IOPAEN				(1U<<2)
#define UART2EN				(1U<<17)
#define DMA1EN				(1U<<0)

#define CCR_EN			(1U<<0)
#define CCR_MINC		(1U<<7)
#define CCR_DIR_FR_MEM	(1U<<4)
#define CCR_TCIE		(1U<<1)

#define CR3_DMAT		(1U<<7)

#define CR1_TE				(1U<<3)
#define CR1_RE				(1U<<2)
#define CR1_UE				(1U<<13)
#define CR1_RXNEIE			(1U<<5)

#define SR_TXE				(1U<<7)
#define SR_RXNE				(1U<<5)

#define SYS_FREQ 			8000000
#define APB1_CLK			SYS_FREQ

#define UART_BAUDRATE		115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

void uart2_write(int ch);

int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
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

void UART2_RX_interrupt_init(void)
{
	/*----------Configure UART GPIO pin----------*/
	/* Enable clock access to GPIOA */
	RCC->APB2ENR |= IOPAEN;

	/* Set PA2 mode to alternate function mode */
	GPIOA->CRL |= (1U<<8);
	GPIOA->CRL |= (1U<<9);
	GPIOA->CRL &=~(1U<<10);
	GPIOA->CRL |= (1U<<11);

	/* Set PA3 mode to alternate function mode */
	GPIOA->CRL &=~(1U<<12);
	GPIOA->CRL &=~(1U<<13);
	GPIOA->CRL &=~(1U<<15);
	GPIOA->CRL |= (1U<<14);

	/*-----------Configure UART module-----------*/
	/* Enable clock access to UART2 */
	RCC->APB1ENR |= UART2EN;

	/* Configure baud rate */
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/* Configure the transfer direction */
	USART2->CR1 |= (CR1_TE | CR1_RE);

	/* Enable RXNE interrupt */
	USART2->CR1 |= CR1_RXNEIE;

	/* Enable UART2 interrupt in NVIC */
	NVIC_EnableIRQ(USART2_IRQn);

	/* Enable UART module */
	USART2->CR1 |= CR1_UE;
}

void UART2_RX_TX_init(void)
{
	/*----------Configure UART GPIO pin----------*/
	/* Enable clock access to GPIOA */
	RCC->APB2ENR |= IOPAEN;

	/* Set PA2 mode to alternate function mode */
	GPIOA->CRL |= (1U<<8);
	GPIOA->CRL |= (1U<<9);
	GPIOA->CRL &=~(1U<<10);
	GPIOA->CRL |= (1U<<11);

	/* Set PA3 mode to alternate function mode */
	GPIOA->CRL &=~(1U<<12);
	GPIOA->CRL &=~(1U<<13);
	GPIOA->CRL &=~(1U<<15);
	GPIOA->CRL |= (1U<<14);

	/*-----------Configure UART module-----------*/
	/* Enable clock access to UART2 */
	RCC->APB1ENR |= UART2EN;

	/* Configure baud rate */
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	/* Configure the transfer direction */
	USART2->CR1 |= (CR1_TE | CR1_RE);

	/* Enable UART module */
	USART2->CR1 |= CR1_UE;
}

void DMA1_CH7_init(uint32_t src, uint32_t dst, uint32_t len)
{
	/* Enable clock access to DMA */
	RCC->AHBENR |= DMA1EN;

	/* Disable DMA1 CH7 */
	DMA1_Channel7->CCR &=~CCR_EN;

	while(DMA1_Channel7->CCR & CCR_EN);

	/* Clear all interrupt flags of CH7 */
	DMA1->IFCR |= (1U<<24);
	DMA1->IFCR |= (1U<<25);
	DMA1->IFCR |= (1U<<26);
	DMA1->IFCR |= (1U<<27);

	/* Set the destination buffer */
	DMA1_Channel7->CPAR = dst;

	/* Set the source buffer */
	DMA1_Channel7->CMAR = src;

	/* Set length */
	DMA1_Channel7->CNDTR = len;

	/* Enable memory increment */
	DMA1_Channel7->CCR |= CCR_MINC;

	/* Configure transfer direction */
	DMA1_Channel7->CCR |= CCR_DIR_FR_MEM;

	/* Enable DMA transfer complete interrupt */
	DMA1_Channel7->CCR |= CCR_TCIE;

	/* Enable DMA1 CH7 */
	DMA1_Channel7->CCR |= CCR_EN;

	/* Enable UART2 Transmitter DMA */
	USART2->CR3 |= CR3_DMAT;

	/* DMA Interrupt enable in NVIC */
	NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}

char uart2_read(void)
{
	/* Make sure the receive register is not empty */
	while(!(USART2->SR & SR_RXNE));

	/* Read data */
	return USART2->DR;
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
