#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "stm32c031xx.h"
#include "uart.h" //I need terminal to see exchanged data
#include "mpu6500.h"
#include "I2C.h"


int16_t acc_x, acc_y, acc_z;  //raw values
float g_acc_x, g_acc_y, g_acc_z; //to be converted values according to appropriate selected resolution

uint8_t data_reg[6];
uint8_t ID = 0;

int main(void)
{
	uart_init();
	mpu_init();
	volatile uint32_t i;

	mpu_read_address(0x75, &ID);



	printf("\n\r =================================\n\r ");
	printf("\n\r =================================\n\r ");
	printf("\n\r ADDRESS OF SLAVE DEVICE IS %d\n\r", ID);
	if (ID == 112)
	{
		printf ("\n\r CORRECT DEVICE ACQUIRED (MPU6500) \n\r");
	}
	else
		{
	printf ("\n\r WRONG DEVICE ACQUIRED (NOT MPU6500) OR AD0 pin of sensor is not at GND!!! Best not to leave it floating\n\r");
		}

	printf("\n\r =================================\n\r ");
	printf("\n\r =================================\n\r ");

	for (i = 0; i < 2000000; i++){};

	while (1)
	{

		mpu_multi_read(0x3B, data_reg);
		/* Rebuilt 16-bit signed values by shifting the High byte left by 8 bits and ORing with Low byte */
		acc_x = (int16_t)((data_reg[0] << 8) | data_reg[1]); //aligning and casting data
		acc_y = (int16_t)((data_reg[2] << 8) | data_reg[3]);
		acc_z = (int16_t)((data_reg[4] << 8) | data_reg[5]);

		/* Converted raw values to g units using MPU-6500 sensitivity at +-4g range (8192 LSB/g) */
		g_acc_x = (float)acc_x / 8192.0;
		g_acc_y = (float)acc_y / 8192.0;
		g_acc_z = (float)acc_z / 8192.0;

		printf ("acc_x: %f, acc_y: %f, acc_z: %f\n\r", g_acc_x, g_acc_y, g_acc_z);
		//printf ("raw_values for acc_x: %d, acc_y: %d, acc_z: %d\n\r", acc_x, acc_y, acc_z);
	}
}
