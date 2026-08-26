#include "exti.h"
#include "gpio.h"
#include "stm32c031xx.h"
#include "uart.h"
#include <stdio.h>


#define line13 (1U << 13)

int main(void)
{

	led_init();
	uart_init();
	usr_btn_exti_init();

	while (1)
	{
		printf("nessun interrupt sta funzionando\n\r");
		for (int i = 0; i < 2400000; i++);
		led_off();
	}


}



static void callback(void)
{
	led_on();
	printf("===============================================================================\n\r");
	printf("===============================================================================\n\r");
	printf("===============================================================================\n\r");
	printf("===============================================================================\n\r");
	printf("===============================================================================\n\r");
	printf("User button has been pressed and interrupt has been generated and handled\n\r");
	printf("===============================================================================\n\r");
	printf("===============================================================================\n\r");
	printf("===============================================================================\n\r");
	printf("===============================================================================\n\r");


}



void EXTI4_15_IRQHandler(void)
{
	if ((EXTI -> RPR1 & line13) != 0) //pending rising edge request
	{

		EXTI -> RPR1 |= (1U << 13);//RPR1 has to be reset via writing '1' via SW
		callback(); //revert to callback function

	}

}



