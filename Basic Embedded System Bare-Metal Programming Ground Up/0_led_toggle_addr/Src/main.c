//Where is the led connected?
//Port:	A
//Pin:	5

#define PERIPH_BASE			(0x40000000UL)

#define APB2PERIPH_OFFSET	(0x00010000UL)
#define APB2PERIPH_BASE		(PERIPH_BASE + APB2PERIPH_OFFSET)
#define AHBPERIPH_OFFSET	(0x00020000UL)
#define AHBPERIPH_BASE		(PERIPH_BASE + AHBPERIPH_OFFSET)

#define GPIOA_OFFSET		(0x00000800UL)
#define GPIOA_BASE			(APB2PERIPH_BASE + GPIOA_OFFSET)
#define RCC_OFFSET			(0x00001000UL)
#define RCC_BASE			(AHBPERIPH_BASE + RCC_OFFSET)

#define RCC_APB2ENR_OFFSET	(0x18UL)
#define RCC_APB2ENR			(*(volatile unsigned int *)(RCC_BASE + RCC_APB2ENR_OFFSET))

#define GPIOA_CRL_OFFSET	(0x00UL)
#define GPIOA_CRL			(*(volatile unsigned int *)(GPIOA_BASE + GPIOA_CRL_OFFSET))

#define GPIOA_ODR_OFFSET	(0x0CUL)
#define GPIOA_ODR			(*(volatile unsigned int *)(GPIOA_BASE + GPIOA_ODR_OFFSET))

#define IOPAEN				(1U<<2)

#define PIN5				(1U<<5)
#define LED_PIN				PIN5

/*
 * (1U<<21)	// Set bit 21 to 1
 * &=~(1U<<20)	// Set bit 20 to 0 */

int main(void)
{
	/* 1. Enable clock access to GPIOA */
	RCC_APB2ENR |= IOPAEN;
	/* 2. Set PA5 as output pin */
	GPIOA_CRL |= (1U<<21);	// Set bit 21 to 1
	GPIOA_CRL &=~(1U<<20);	// Set bit 20 to 0
	GPIOA_CRL &=~(1U<<22);
	GPIOA_CRL &=~(1U<<23);
	while(1)
	{
		/* 3. Set PA5 high */
		//GPIOA_ODR |= LED_PIN;
		/* 4. Experiment 2: toggle PA5*/
		GPIOA_ODR ^= LED_PIN;
		for(int i=0; i<100000; i++);
	}
}

