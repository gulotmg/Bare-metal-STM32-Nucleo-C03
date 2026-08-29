#ifndef PWM_H_
#define PWM_H_

#include "stm32c031xx.h"

void TIM_init(void);
uint16_t PWM_control(void);

#endif /* PWM_H_ */
