#include "adc.h"
#include "uart.h"


/* Macros to improve readability */

#define GPIOAEN 	(1U << 0)
#define ADCEN		(1U << 20)
#define ADEN 		(1U << 0)
#define ADSTART 	(1U << 2)
#define DISCEN 		(1U << 16)
#define CONT 		(1U << 13)
#define SR_EOC		(1U << 2)



void init_pa1_adc (void) {

	/* initialize PA1 as ADC */

	//clock enable for GPIOA

	RCC -> IOPENR |= (GPIOAEN);

	//setting the mode for PA1 as analog in GPIOA

	GPIOA -> MODER |= (1U<<2);
	GPIOA -> MODER |= (1U<<3);

	//clock enable for ADC module

	RCC -> APBENR2 |= (ADCEN);

	//channel select (this is crucial otherwise it does not work!!!)
	//this is because PA1 is linked to ADC_IN1 which is connected to channel 1 (see ref manual page 296)

	ADC1 -> CHSELR |= (1U<<1);

	//activating the ADC


	ADC1 -> CR |= (ADEN);



}


void conversion_start (void){



	//Disable discontinuous
	ADC1 -> CFGR1 &= ~(DISCEN);

	//enable continuous

	ADC1 -> CFGR1 |= (CONT);

	//start conversion command

	ADC1 -> CR |= (ADSTART);

}



uint32_t ADC_read(void){

	//waiting for end of conversion by looking at status register

	while (!(ADC1 -> ISR & SR_EOC )) {/*waiting for EOC to be 1, if so
	 	 	 	 	 	 	 	   then while goes false and I move to
	 	 	 	 	 	 	 	   next instruction*/}


	//write inside the DATA register (data will be passed via UART and taken from here)
	return (ADC1 -> DR);



}

