#ifndef ADC__

#define ADC__

#include <stdint.h>
#include "stm32c031xx.h"

// functions
void init_pa1_adc (void);
void conversion_start (void);
uint32_t ADC_read(void);




#endif
