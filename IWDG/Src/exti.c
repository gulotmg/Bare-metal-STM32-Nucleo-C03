#include "exti.h"

void usr_btn_exti_init(void)
{

	__disable_irq();
	//activate clock and initialize PC13 (user button)
	RCC -> IOPENR |= (1U << 2);

	//set input mode for PC13
	GPIOC -> MODER &= ~(1U<<26); GPIOC -> MODER &= ~(1U<<27);

	//selecting PORTC in EXTI
	EXTI -> EXTICR[3] &= ~(0xFFU << 8);
	EXTI -> EXTICR[3] |= (0x02U << 8);

	/*Unmasking (this line of interrupts is maskable, so we need to make sure logic does not ignore it);
	  useful for debouncing for instance.*/
	EXTI -> IMR1 |= (1U << 13);

	//select rising edge trigger
	EXTI -> RTSR1 |= (1U << 13);

	//shared interrupt line in the NVIC
	NVIC_EnableIRQ(EXTI4_15_IRQn);

	__enable_irq();
}


