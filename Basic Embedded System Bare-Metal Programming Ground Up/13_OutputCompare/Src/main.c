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

int main(void)
{
	TIM2_PA0_output_compare();
	while(1)
	{

	}
}
