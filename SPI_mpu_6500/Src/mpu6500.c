#include "mpu6500.h"


//only accelerometer data (no gyro)



void mpu_write(uint8_t address, uint8_t value){

	uint8_t data[2];
	data[0] = address | 0x00; //setting WRITE operation

	data[1] = value;

	cs_enable(); //starting communication by pulling CS low

	spi1_transmitter(data, 2); //sending 2 bytes (address and data), notice data is a pointer

	cs_disable(); //stopping configuration by pulling CS high

}


void mpu_read (uint8_t address, uint8_t *value){

	address |= 0x80;//setting READ operation (next byte that comes from MOSI is ignored)

	cs_enable();

	//sending address
	spi1_transmitter(&address,1); //using '&' since this is not a array (warning can be ignored)

	spi1_receiver(value, 6); //reading only the 6 registers in accelerometer

	cs_disable();


}


void mpu_init(void){

	uint32_t i=0;
	gpio_init();
	spi1_config();

	mpu_write(0x6B, 0x80); //suggested hardware reset

	//suggested software delay
	for (i = 0; i < 1200000; i++){}

	mpu_write(0x68, 0x07); //signal path reset

	//suggested software delay
	for (i = 0; i < 1200000; i++){}

	mpu_write(0x6A, 0x10); //setting SPI mode only

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
