#ifndef MPU6500_H_
#define MPU6500_H_

#include "spi.h"
#include <stdint.h>

#define I2C_IF_DIS (0x40)

void mpu_init(void);
void mpu_read (uint8_t address, uint8_t *value);
void mpu_write(uint8_t address, uint8_t value);

#endif /* MPU6500_H_ */
