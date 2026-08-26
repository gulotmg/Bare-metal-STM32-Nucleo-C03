#include "rtc.h"
#include <stdint.h>
#include <stdio.h>

void rtc_init(void)
{

	//enable clock to RTC
	RCC -> APBENR1 |= (RCC_APBENR1_RTCAPBEN);
	//  Enable LSE Oscillator and wait for it to stabilize
	RCC->CSR1 |= (1U << 0);     // LSEON: Turn on Low-Speed External oscillator

	while (!(RCC->CSR1 & (1U << 1))) {
		// Wait until LSERDY is set by hardware (LSE is stable)
	}

	// Select LSE as RTC clock source (RTCSEL = 01)
	RCC->CSR1 |= (1U << 8);     // Set RTCSEL (bit 8) to 1
	RCC->CSR1 &= ~(1U << 9);    // Clear RTCSEL[1] (bit 9) to 0

	//  Enable the RTC clock
	RCC->CSR1 |= (1U << 15);    // RTCEN: Enable RTC clock

	//  Disable RTC registers write protection using the safety key sequence
	RTC->WPR = 0xCAU;           // Write Key 1 to unlock WPR
	RTC->WPR = 0x53U;           // Write Key 2 to unlock WPR

	// Enter RTC Initialization Mode
	RTC->ICSR |= (1U << 7);     // Set INIT bit to 1 to request initialization mode
	while (!(RTC->ICSR & (1U << 6))) {
		// Poll INITF bit (bit 6) until hardware confirms entry (takes ~2 RTCCLK cycles)
	}

	// Configure RTC Prescalers (PRER) for LSE (32.768 kHz)
	// Asynchronous Divisor: PREDIV_A + 1 = 127 + 1 = 128
	// Synchronous Divisor:  PREDIV_S + 1 = 255 + 1 = 256
	// Total division factor: 128 * 256 = 32768, which yields exactly 1 Hz
	RTC->PRER = (127U << 16) | (255U << 0);

	// Configure initial time shadow register
	RTC->TR = (0U << RTC_TR_PM_Pos)   | // 24h format (PM is unused)
	          (1U << RTC_TR_HT_Pos)   | // Hour Tens (1)
	          (9U << RTC_TR_HU_Pos)   | // Hour Units (9) -> Hour: 19
	          (1U << RTC_TR_MNT_Pos)  | // Minute Tens (1)
	          (1U << RTC_TR_MNU_Pos)  | // Minute Units (1) -> Minute: 11
	          (2U << RTC_TR_ST_Pos)   | // Second Tens (2)
	          (3U << RTC_TR_SU_Pos);    // Second Units (3) -> Second: 23

	// Configure initial date shadow register (Friday, August 14th, 2026)
	RTC->DR = (2U << RTC_DR_YT_Pos)   | // Year Tens (2)
	          (6U << RTC_DR_YU_Pos)   | // Year Units (6) -> Year: 26 (2026)
	          (5U << RTC_DR_WDU_Pos)  | // Weekday (5 = Friday)
	          (0U << RTC_DR_MT_Pos)   | // Month Tens (0)
	          (8U << RTC_DR_MU_Pos)   | // Month Units (8) -> Month: August
	          (1U << RTC_DR_DT_Pos)   | // Date Tens (1)
	          (4U << RTC_DR_DU_Pos);    // Date Units (4) -> Date: 14

	//  Select 24-hour format
	RTC->CR &= ~(1U << 6);      // Clear FMT bit in RTC_CR to enforce 24h format

	//  Exit RTC Initialization Mode to start the calendar counter
	RTC->ICSR &= ~(1U << 7);    // Clear INIT bit

	//  Re-enable write protection to secure RTC registers against corruption
	RTC->WPR = 0xFFU;           // Write an invalid key to lock WPR
}


//creating a function to convert BCD to DECIMAL so it is human readable.
uint8_t bcd_to_dec(uint8_t bcd_value)
{
	return (uint8_t) (((uint8_t)(bcd_value & 0xF0U) >> 4U) * 10) + ((uint8_t)(bcd_value & 0x0FU));
}

//creating functions to extract data from DR and TR
uint8_t hours_extractor(void)
{
	return (uint8_t) ((RTC -> TR >> 16U) & 0x3FU); /*this extracts the correct bits with a right shift. Similarly for the others. Note that the fact a casting is
													being used takes the last 8 bits. That's the reason for the right shift, since this puts the bits of interest
													at the position [0:7], which the casting selects (since it takes starting from LSB) */
}

uint8_t minutes_extractor(void)
{
	return (uint8_t) ((RTC -> TR >>  8U) & 0x7FU);
}

uint8_t seconds_extractor(void)
{
	return (uint8_t) ((RTC -> TR>> 0U) & 0x7FU);
}

uint8_t year_extractor(void)
{
	return (uint8_t) ((RTC -> DR >> 16U) & 0xFFU);
}

uint8_t weekday_extractor(void)
{
	return (uint8_t) ((RTC -> DR >> 13U) & 0x07U);
}

uint8_t month_extractor(void)
{
	return (uint8_t) ((RTC -> DR >> 8U) & 0x1FU);
}

uint8_t date_extractor(void)
{
	return (uint8_t) ((RTC -> DR >> 0U) & 0x3FU);
}


