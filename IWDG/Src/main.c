#include "uart.h"
#include "exti.h"
#include "wdg.h"
#include "gpio.h"
#include <stdio.h>

// Volatile is mandatory for variables shared with ISR (never forget)
volatile uint8_t state = 0;
volatile uint8_t btn_event_flag = 0;

#define line13 (1U << 13)

int main(void)
{
    uart_init();
    IWDG_init();
    usr_btn_exti_init();

    printf("\r\n=== SYSTEM RESET OR FIRST BOOT ===\r\n");
    printf("System is entering NORMAL state (State 0)\r\n\n");

    while (1)
    {
        // ==========================================
        // EVENT HANDLING (FSM Transitions)
        // ==========================================
        if (btn_event_flag == 1)
        {
            btn_event_flag = 0; /*Clear the event flag immediately, so we then go
            					  to state execution part */

            /* Toggle state (0 becomes 1, 1 becomes 0; at event, state
             * changes by comparing '1' with previous one)*/
            state ^= 1;

            if (state == 1)
            {
                led_on();
                printf("\r\n[!] Button pressed -> Entering FAULT state\r\n");
            }
            else
            {
                led_off();
                printf("\r\n[*] Button pressed -> Recovering to NORMAL state\r\n");
            }
        }

        // ==========================================
        // STATE EXECUTION
        // ==========================================
        if (state == 0)
        {
            // NORMAL OPERATION: Feed the watchdog
            IWDG->KR = 0x0000AAAAU;

            // Volatile prevents compiler from optimizing the delay away
            for (volatile int i = 0; i < 1800000; i++) {}
            printf("System OK - Watchdog fed.\n\r");
        }
        else
        {
            // FAULT SIMULATION: Starve the watchdog
            printf("FAULT STATE - Watchdog is starving! Reset incoming...\n\r");

            for (volatile int i = 0; i < 1800000; i++) {}
        }
    }
}

void EXTI4_15_IRQHandler(void)
{
    // Check if the interrupt is from line 13
    if ((EXTI->RPR1 & line13) != 0)
    {
        // RM0490 Reference Manual: Cleared by writing 1 to the corresponding bit
        EXTI->RPR1 = line13;

        // Just set the flag and exit
        btn_event_flag = 1;
    }
}
