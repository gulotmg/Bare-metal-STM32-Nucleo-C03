#ifndef I2C_H_
#define I2C_H_

#include "stm32c031xx.h"
#include <stdint.h>
#include <stdio.h>

//functions
void i2c1_init(void);
void i2c1_multi_byte_read (uint8_t address,uint32_t k,uint8_t *data);
void i2c1_multi_write(uint8_t address,uint32_t k,uint8_t *data);


#endif /* I2C_H_ */
