#include "DMA.h"
#include <stdint.h>

void DMA_init(void) {

    //  Enable DMA1 peripheral clock in the RCC AHBENR register (RM0490 Section 5.4.10)
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    //  Ensure the DMA channel is disabled before modifying its configuration registers
    DMA1_Channel1->CCR &= ~DMA_CCR_EN;

    //  Configure Peripheral Address (Source)
    // The source is the ADC1 Data Register (32 bit address...)
    DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;

    //  Configure Peripheral Address (Destination)
    // The destination is the TIM1 -> CCR1 register.
    DMA1_Channel1->CMAR = (uint32_t)&TIM1 -> CCR1;

    /* Configure DMAMUX to connect ADC1 request to DMA1_Channel1 (in DMAMUX 0 corresponds to 1 and so on).
     * Avoiding to do so implies that the DMArequests of the ADC never get to the DMAc... which implies silence
     * from the whole system, even if everything else is well configured.
     */

    DMAMUX1_Channel0 -> CCR |= (0x05U << 0);

    // Configure the Number of Data to Transfer (NDT)
    DMA1_Channel1->CNDTR = 1U;

    //  Clear the entire Channel Configuration Register (CCR) for a clean state
    DMA1_Channel1->CCR = 0U;

    //  Configure Channel Control Register (CCR) Bitfields

    // Set Priority Level to High (PL = 01).
    DMA1_Channel1->CCR |= DMA_CCR_PL_0;

    // Set Memory Data Size to 32-bit (Word) -> MSIZE = 10
    DMA1_Channel1->CCR |= DMA_CCR_MSIZE_1;

    // Set Peripheral Data Size to 32-bit (Word) -> PSIZE = 10
    DMA1_Channel1->CCR |= DMA_CCR_PSIZE_1;

    // Peripheral(s) Increment Mode (PINC) remains 0 (Disabled).
    // This is because we always read from the exact same hardware address (&ADC1->DR) and write to the same (&TIM1 -> CCR1).

    // Direction (DIR) remains 0 (Peripheral to Peripheral).

    // Circular Mode (CIRC) is activated
    DMA1_Channel1 -> CCR |= DMA_CCR_CIRC;

    // Memory-to-Memory (MEM2MEM) remains 0 (Disabled).

    //Finally, enable the DMA channel
    DMA1_Channel1->CCR |= DMA_CCR_EN;
}



