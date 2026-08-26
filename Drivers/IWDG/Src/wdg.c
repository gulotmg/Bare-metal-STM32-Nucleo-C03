#include "wdg.h"


void IWDG_init(void)
{

	RCC -> CSR2 |= (1U << 0);     // LSION: Turn on Low-Speed internal oscillator (which is used by WDG)
	while (!(RCC->CSR2 & (1U << 1))); //wait for LSI to be turned on

	//starting the watchdog
	IWDG -> KR = (0x0000CCCC);

	//disable write protection
	IWDG -> KR = (0x00005555);

	//write the prescaler to /256
	while (IWDG->SR & (1U << 0));
	IWDG -> PR |= (0x06U << 0U);

	//load the reload register
	while (IWDG->SR & (1U << 1));
	IWDG -> RLR |= (0xFFFU << 0U);
	while (IWDG -> SR & (0x01U << 1U)){};


	//make sure update is done
	while (IWDG -> SR & (0x01U << 0U)){};
	while (IWDG -> SR & (0x01U << 1U)){};
	while (IWDG -> SR & (0x01U << 2U)){};

	//reload the counter value (which also reactivates write protection)
	IWDG -> KR = (0x0000AAAA);

}
