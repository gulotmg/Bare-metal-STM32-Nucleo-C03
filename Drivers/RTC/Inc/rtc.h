#ifndef RTC_H_
#define RTC_H_

#include "stm32c031xx.h"

void rtc_init(void);
uint8_t bcd_to_dec(uint8_t bcd_value);
uint8_t hours_extractor(void);
uint8_t minutes_extractor(void);
uint8_t seconds_extractor(void);
uint8_t year_extractor(void);
uint8_t weekday_extractor(void);
uint8_t month_extractor(void);
uint8_t date_extractor(void);

#endif /* RTC_H_ */
