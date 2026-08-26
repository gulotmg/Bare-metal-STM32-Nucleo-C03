#include "gpio.h"


void led_init(void)
{

	//activate clock to GPIOA
	RCC -> IOPENR |= (1U << 0);

	//setting mode to output in PA5
	GPIOA -> MODER |= (1U << 10); GPIOA -> MODER &= ~(1U << 11);

}


void led_on (void)
{
	GPIOA -> ODR |= (1U << 5);
}

void led_off (void)
{
	GPIOA -> ODR &= ~(1U << 5);
}



