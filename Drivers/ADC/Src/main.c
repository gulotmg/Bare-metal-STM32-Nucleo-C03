#include <stdio.h>
#include "adc.h"
#include "uart.h"

//includere uart (scrivere driver)


int sensor_sample;

int main (void){

	//init aurt
	uart_init();


	//initialize ADC
	init_pa1_adc ();


	//start ADC in continous mode
	conversion_start();

	while (1) {

		sensor_sample = ADC_read();


		printf ("Sensor value is %d\r\n", sensor_sample);


	}


}

