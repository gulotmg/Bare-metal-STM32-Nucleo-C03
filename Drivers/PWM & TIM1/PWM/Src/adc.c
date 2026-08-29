#include "stm32c031xx.h"
/* Macros to improve readability */

#define GPIOAEN 	(1U << 0)
#define ADCEN		(1U << 20)
#define ADEN 		(1U << 0)
#define ADSTART 	(1U << 2)
#define DISCEN 		(1U << 16)
#define CONT 		(1U << 13)
#define SR_EOC		(1U << 2)
void init_pa1_adc(void)
{
    /* Enable GPIOA and ADC1 peripheral clocks */
    RCC->IOPENR  |= RCC_IOPENR_GPIOAEN;
    RCC->APBENR2 |= RCC_APBENR2_ADCEN;

    /* Configure PA1 in analog mode (ADC_IN1) */
    GPIOA->MODER |= (3U << (1 * 2));

    /* Set synchronous ADC clock: PCLK/2 */
    ADC1->CFGR2 &= ~ADC_CFGR2_CKMODE;
    ADC1->CFGR2 |= ADC_CFGR2_CKMODE_0;

    /* Enable voltage regulator and wait for stabilization (~20us) */
    ADC1->CR |= ADC_CR_ADVREGEN;
    for (volatile int i = 0; i < 1000; i++) {}

    /* Start ADC self-calibration (must be executed with ADEN = 0) */
    ADC1->CR |= ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL);

    //DMA AND CIRCULAR. NOTE: YOU MUST HAVE, in this case, both DMA1 and ADC1 set at circular mode!
    ADC1->CFGR1 |= ADC_CFGR1_DMAEN;
    ADC1->CFGR1 |= ADC_CFGR1_DMACFG;

    // Disable discontinuous
    ADC1->CFGR1 &= ~(DISCEN);

    // Enable continuous
    ADC1->CFGR1 |= (CONT);


    //activate oversampler x4
    ADC1->CFGR2 &= ~(ADC_CFGR2_OVSR | ADC_CFGR2_OVSE); // Clear previous settings
    ADC1->CFGR2 |= ADC_CFGR2_OVSE;                      // Enable Oversampler
    ADC1->CFGR2 |= (1U << ADC_CFGR2_OVSR_Pos);          // OVSR = 001 -> 4x raw adc 12 bit

    /* Configure sampling time: 12.5 ADC clock cycles on SMP1, select SMP1 for CH1 */
    ADC1->SMPR &= ~ADC_SMPR_SMP1;
    ADC1->SMPR |= (3U << ADC_SMPR_SMP1_Pos);
    ADC1->SMPR &= ~ADC_SMPR_SMPSEL1;

    /* Select Channel 1 (PA1) and wait for CCRDY */
    ADC1->CHSELR |= ADC_CHSELR_CHSEL1;
    while (!(ADC1->ISR & ADC_ISR_CCRDY));
    ADC1->ISR |= ADC_ISR_CCRDY;

    /* Enable ADC and wait for ADRDY flag */
    ADC1->ISR |= ADC_ISR_ADRDY; // Clear ADRDY
    ADC1->CR  |= ADC_CR_ADEN;   // Enable ADC

    // wait for ADC to be ready
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0);


    /* Arming ADC to listen for incoming hardware trigger pulses MUST be done after DMA is enabled
     * See main.c
     * */

}
