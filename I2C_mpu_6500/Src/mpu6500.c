#include "mpu6500.h"

// Only accelerometer data (no gyro)
// AD0 is put to ground

/* Correct 7-bit I2C Slave Address for MPU-6500 when AD0 is grounded */
#define MPU6500_I2C_ADDR   0x68

void mpu_write(uint8_t reg, uint8_t value)
{
	uint8_t tx_data[2];

	tx_data[0] = reg;   // First byte is the target register address
	tx_data[1] = value; // Second byte is the value to be written

	/* Send 2 bytes to the actual MPU-6500 I2C address */
	i2c1_multi_write(MPU6500_I2C_ADDR, 2, tx_data);
}


void mpu_read_address (uint8_t reg, uint8_t *data)
{
	/* Step 1: Write the register address we want to read from */
	i2c1_multi_write(MPU6500_I2C_ADDR, 1, &reg);

	/* Step 2: Read 1 byte of data from the device */
	i2c1_multi_byte_read(MPU6500_I2C_ADDR, 1, data);
}

void mpu_multi_read (uint8_t reg, uint8_t *data)
{
	/* Step 1: Write the starting register address (e.g., 0x3B) */
	i2c1_multi_write(MPU6500_I2C_ADDR, 1, &reg);

	/* Step 2: Read 6 consecutive bytes (ACCEL_XOUT_H to ACCEL_ZOUT_L) */
	i2c1_multi_byte_read(MPU6500_I2C_ADDR, 6, data);
}

void mpu_init(void){

	volatile uint32_t i = 0; // Volatile prevents compiler optimization from deleting delay loops
	i2c1_init();

	mpu_write(0x6B, 0x80); // Suggested hardware reset

	// Suggested software delay for hardware reset (100ms)
	for (i = 0; i < 1200000; i++){}

	mpu_write(0x68, 0x07); // Signal path reset

	// Suggested software delay for signal path reset (100ms)
	for (i = 0; i < 1200000; i++){}

	// Wake up MPU-6500 and select auto clock source (best PLL)
	mpu_write(0x6B, 0x01);

	for (i = 0; i < 1200000; i++){}

	// Disable X, Y, Z gyroscope channels to save power and keep only Accelerometer on
	mpu_write(0x6C, 0x07);

	// Set Accelerometer Full Scale range to +-4g (ACCEL_FS_SEL = 01)
	mpu_write(0x1C, 0x08);

	// Set Accelerometer Digital Low Pass Filter (DLPF) to 41Hz bandwidth
	mpu_write(0x1D, 0x03);

}
