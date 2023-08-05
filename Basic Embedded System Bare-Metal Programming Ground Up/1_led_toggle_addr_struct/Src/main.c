//Where is the led connected?
//Port:	A
//Pin:	5

#include <stdint.h>

#define PERIPH_BASE			(0x40000000UL)

#define APB2PERIPH_OFFSET	(0x00010000UL)
#define APB2PERIPH_BASE		(PERIPH_BASE + APB2PERIPH_OFFSET)
#define AHBPERIPH_OFFSET	(0x00020000UL)
#define AHBPERIPH_BASE		(PERIPH_BASE + AHBPERIPH_OFFSET)

#define GPIOA_OFFSET		(0x00000800UL)
#define GPIOA_BASE			(APB2PERIPH_BASE + GPIOA_OFFSET)
#define RCC_OFFSET			(0x00001000UL)
#define RCC_BASE			(AHBPERIPH_BASE + RCC_OFFSET)

#define IOPAEN				(1U<<2)

#define PIN5				(1U<<5)
#define LED_PIN				PIN5

typedef struct
{
	volatile uint32_t DUMMY[6];
	volatile uint32_t APB2ENR;	/*!<APB2 peripheral clock enable register,	Address offset: 0x18 */
}RCC_TypeDef;

typedef struct
{
	volatile uint32_t CRL;		/*!< GPIO port configuration register low,	Address offset: 0x00 */
	volatile uint32_t CRH;		/*!< GPIO port configuration register high,	Address offset: 0x04 */
	volatile uint32_t DUMMY;
	volatile uint32_t ODR;		/*!< GPIO port output data register,		Address offset: 0x0C */
}GPIO_TypeDef;

#define RCC					((RCC_TypeDef*) RCC_BASE)
#define GPIOA				((GPIO_TypeDef*) GPIOA_BASE)

int main(void)
{
	/* 1. Enable clock access to GPIOA */
	RCC->APB2ENR |= IOPAEN;

	/* 2. Set PA5 as output pin */
	GPIOA->CRL |= (1U<<21);
	GPIOA->CRL &=~(1U<<20);
	GPIOA->CRL &=~(1U<<22);
	GPIOA->CRL &=~(1U<<23);

	while(1)
	{
		/* 4. Experiment 2: toggle PA5*/
		GPIOA->ODR ^= LED_PIN;
		for(int i=0; i<100000; i++);
	}
}

