#include "rtc.h"
#include "uart.h"
#include <stdio.h>
#include <time.h>


char* weekday_decoder (void)
{
	if (weekday_extractor() == 1U) return ("Monday");
	else if (weekday_extractor() == 2U) return ("Tuesday");
	else if (weekday_extractor() == 3U) return ("Wednesday");
	else if (weekday_extractor() == 4U) return ("Thursday");
	else if (weekday_extractor() == 5U) return ("Friday");
	else if (weekday_extractor() == 6U) return ("Saturday");
	else if (weekday_extractor() == 7U) return ("Sunday");
	else return ("Undefined");
}




int main(void)

{

	uart_init();
	rtc_init();

	while (1)
	{
		printf("Time is %d : %d : %d ||", bcd_to_dec(hours_extractor()), bcd_to_dec(minutes_extractor()),
				bcd_to_dec(seconds_extractor()));

		printf("Date is %d - %d - %d ||", bcd_to_dec(date_extractor()), bcd_to_dec(month_extractor()),
						bcd_to_dec(year_extractor()));

		printf("Weekday is %s\n\r", weekday_decoder());

b
	}

}

