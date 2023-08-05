#include "stm32f1xx.h"
#include "systick.h"

#define SYSTICK_LOAD_VAL		8000
#define CTRL_ENABLE				(1U<<0)
#define CTRL_CLKSOURCE			(1U<<2)
#define CTRL_COUNTFLAG			(1U<<16)
#define CTRL_TICKINT			(1U<<1)

#define ONE_SEC_LOAD			8000000


void systickDelayMs(int delay)
{
	/* Reload with number of clocks per millisecond */
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	/* Clear systick current value register */
	SysTick->VAL = 0;

	/* Enable systick and select internal clock source */
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSOURCE;

	for(int i=0; i<delay; i++)
	{
		/* Wait until the count flag is set */
		while(!(SysTick->CTRL & CTRL_COUNTFLAG));
	}
	SysTick->CTRL = 0;
}

void systick_1Hz_Interrupt(void)
{
	/* Reload with number of clocks per second */
	SysTick->LOAD = ONE_SEC_LOAD - 1;

	/* Clear systick current value register */
	SysTick->VAL = 0;

	/* Enable systick and select internal clock source */
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSOURCE;

	/* Enable systick interrupt */
	SysTick->CTRL |= CTRL_TICKINT;
}
