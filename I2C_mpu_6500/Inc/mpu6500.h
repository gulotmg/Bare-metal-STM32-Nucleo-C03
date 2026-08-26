#ifndef MPU6500_H_
#define MPU6500_H_

#include "I2C.h"
#include <stdint.h>

void mpu_init(void);
void mpu_multi_read (uint8_t address, uint8_t *value);
void mpu_write(uint8_t address, uint8_t value);
void mpu_read_address(uint8_t reg, uint8_t *data);

#endif /* MPU6500_H_ */
