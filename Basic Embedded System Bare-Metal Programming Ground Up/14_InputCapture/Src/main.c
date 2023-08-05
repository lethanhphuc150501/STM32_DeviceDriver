#include <stm32f1xx.h>
#include <stm32f103xb.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"


#define PIN5		(1U<<5)
#define LED_PIN		PIN5


int timestamp = 0;
int time = 0;

int main(void)
{
	TIM2_PA0_output_compare();
	TIM3_PA6_input_capture();

	while(1)
	{
		/* Wait until edge is captured */
		//while(!(TIM3->SR & SR_CC1IF));

		/* Read captured value */
		timestamp = TIM3->CCR1;
		time = TIM3->CNT;
	}
}
