#include "PWM.h"
#include "stm32c031xx.h"
#include <stdint.h>

#define PWM_ARR 0x2EE0U
#define PWM_50  (PWM_ARR / 2U)


void TIM_init(void) {
    // Enable clocks
    RCC->IOPENR  |= RCC_IOPENR_GPIOAEN;
    RCC->APBENR2 |= RCC_APBENR2_TIM1EN;

    // PA8: AF mode (Clear then set)
    GPIOA->MODER &= ~(0x3U << 16);
    GPIOA->MODER |=  (0x2U << 16);

    // PA8: AF2 (TIM1_CH1) (Clear then set)
    GPIOA->AFR[1] &= ~(0xFU << 0);
    GPIOA->AFR[1] |=  (0x2U << 0);

    // Timebase
    TIM1->PSC  = 0;
    TIM1->ARR  = PWM_ARR;
    TIM1->CCR1 = PWM_50;

    // PWM Mode 1, preload enabled
    TIM1->CCMR1 = (6U << 4) | (1U << 3);

    // Output enabled, active high
    TIM1->CCER = TIM_CCER_CC1E;

    // Force shadow register update
    TIM1->EGR = TIM_EGR_UG;

    // Main Output Enable (TIM1 specific)
    TIM1->BDTR = TIM_BDTR_MOE;

    // Start counter
    TIM1->CR1 = TIM_CR1_CEN;
}

uint16_t PWM_control(void) {
    uint32_t current_ccr = TIM1->CCR1;
    if (current_ccr >= PWM_ARR) {
        return 100U;
    }
    return (uint16_t)((current_ccr * 100U) / PWM_ARR);
}
