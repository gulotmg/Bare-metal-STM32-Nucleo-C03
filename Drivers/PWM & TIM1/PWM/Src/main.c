#include <stdio.h>
#include "adc.h"
#include "uart.h"
#include "PWM.h"
#include "DMA.h"

#define PWM_ARR 0x2EE0U
#define ADSTART 	(1U << 2)


//simple DMA implementation is present as well. Can definitely be extended more.

int main (void){

	//init aurt
	uart_init();

	//initialize ADC
	init_pa1_adc ();

	//initialize DMA and enable ADC
	DMA_init();

	//init TIM1 for PWM
	TIM_init();

	ADC1 -> CR |= (ADSTART);
	while (1) {
		//quick way to cover up to 100% duty cycle WITHOUT oversampling: ADC is 12 bit while TIM1 ARR is 16 bit.
		//PWM_control()*3-285;

		//with oversampling previous commented part is not required and is actually done via hardware without stressing the cpu
		PWM_control();
		printf("Duty Cycle: %u%%\n\r", PWM_control());
	}
}

