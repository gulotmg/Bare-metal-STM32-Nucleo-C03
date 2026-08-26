#include "I2C.h"

void i2c1_init(void){

	/*
	 *	PA9 SCL, PA10 SDA
	 */

	//setting HSION
	RCC -> CR |= (1U<<8);
	// Wait until HSI48 is ready
	while (!(RCC->CR & (1U << 10)));
	//division factor of 6
	RCC -> ICSCR |= (1U<<5); RCC -> ICSCR &= ~(1U<<6); RCC -> ICSCR |= (1U<<7);
	//selecting HSIKER for I2C1
	RCC -> CCIPR |= (1U<<13); RCC -> CCIPR &= ~(1U<<12);

	//clock access to GPIOA
	RCC -> IOPENR |= (1U<<0);

	//setting mode AF
	GPIOA -> MODER |= (1U<<19); GPIOA -> MODER &= ~(1U << 18);
	GPIOA -> MODER |= (1U<<21); GPIOA -> MODER &= ~(1U << 20);

	//setting to appropriate AF6
	GPIOA -> AFR[1] &= ~(1U<<7); GPIOA -> AFR[1] |= (1U<<6);
	GPIOA -> AFR[1] |= (1U<<5); GPIOA -> AFR[1] &= ~(1U<<4);

	GPIOA -> AFR[1] &= ~(1U<<11); GPIOA -> AFR[1] |= (1U<<10);
	GPIOA -> AFR[1] |= (1U<<9); GPIOA -> AFR[1] &= ~(1U<<8);

	/*setting open-drain mode since in I2C buses are shared and open-drain is immune
	 * to bus contention issues. Since this disables push-pull mode this implies that
	 * I have to set a pull-up mode manually. This is done for both pins.
	 */

	//this configures open-drain mode for both pins
	GPIOA -> OTYPER |= (1U<<9); GPIOA -> OTYPER |= (1U<<10);

	//this activates pull up resistors
	GPIOA -> PUPDR &= ~(1U<<19); 	GPIOA -> PUPDR |= (1U<<18);
	GPIOA -> PUPDR &= ~(1U<<21);	GPIOA -> PUPDR |= (1U<<20);

	//activate clock to I2C1, refer to page 685 of Reference manual "I2C initialization flow"
	RCC -> APBENR1 |= (1U<<21);

	I2C1 -> CR1 &= ~(1U<<0); //deactivate PE
	I2C1 -> CR1 &= ~(1U<<12); //Activate analog filer
	I2C1 -> CR1 &= ~(1U<<11); I2C1 -> CR1 &= ~(1U<<10); I2C1 -> CR1 &= ~(1U<<9); I2C1 -> CR1 &= ~(1U<<8); //deactivate digital filter

	//set 7 bit addressing mode
	I2C1 -> CR2 &= ~(1U<<11);
	//configuring timings for fast mode see example table 112
	//I2C1 -> TIMINGR = (0x0U << 28) | (0x3U<<20) | (0x1U << 16) | (0x3U << 8) | (0x9U << 0);
	I2C1 -> TIMINGR = (0x1U << 28) | (0x4U << 20) | (0x2U << 16) | (0x0FU << 8) | (0x13U << 0);


	I2C1 -> CR1 &= ~(1U<<17); //disable clock stretching
	I2C1 -> CR1 |= (1U<<0); //activate PE

}

void i2c1_multi_byte_read (uint8_t slave_address, uint32_t k, uint8_t *data){

    /* Wait until the bus is free (BUSY = 0) */
    while (I2C1 -> ISR & (1U << 15)) {}

    /*
     * Configure the Master transfer parameters in CR2:
     * - (address << 1): Shift 7-bit address to fit SADD[7:1]
     * - (k << 16): Number of bytes to receive (NBYTES)
     * - (1U << 10): Set RD_WRN to 1 (Read Operation)
     * - (1U << 25): Enable AUTOEND to automatically generate STOP condition
     * - (1U << 13): Set START bit to initiate transaction
     */

    I2C1 -> CR2 = ((uint32_t)slave_address << 1) | (k << 16) | (1U << 10) | (1U << 25) | (1U << 13);

    /* Read loop for receiving k bytes */
    for (uint32_t i = 0; i < k; i++) {
        /* Wait until receive data register is not empty (RXNE = 1) */
        while (!(I2C1 -> ISR & (1U << 2))) {}

        /* Store received byte in data buffer */
        data[i] = I2C1 -> RXDR;
    }

    /* Wait for the automatic STOP condition to complete (STOPF = 1) */
    while (!(I2C1 -> ISR & (1U << 5))) {}

    /* Clear the STOP detection flag */
    I2C1 -> ICR |= (1U << 5);
}

void i2c1_multi_write(uint8_t slave_address, uint32_t k, uint8_t *data)
{
    /* Wait until the bus is free (BUSY = 0) */
    while (I2C1 -> ISR & (1U << 15)) {}

    /*
     * Configure the Master transfer parameters in CR2:
     * - (address << 1): Shift 7-bit address to fit SADD[7:1]
     * - (k << 16): Number of bytes to transmit (NBYTES)
     * - RD_WRN is set to 0 (Write Operation)
     * - (1U << 25): Enable AUTOEND to automatically generate STOP condition
     * - (1U << 13): Set START bit to initiate transaction
     */

    I2C1 -> CR2 = ((uint32_t)slave_address << 1) | (k << 16) | (1U << 25) | (1U << 13);

    /* Write loop for transmitting k bytes */
    for (uint32_t i = 0; i < k; i++)
    {

        /*
         * Wait until transmit data register is empty and ready (TXIS = 1).
         * Also check for Acknowledge Failure (NACKF = 1) in case the slave rejects the byte.
         */

        while (!(I2C1 -> ISR & (1U << 1))) //this evaluates when the next TXDR is empty (if empty loop breaks)
        {
            if (I2C1 -> ISR & (1U << 4))
            { // NACKF detected
                I2C1 -> ICR |= (1U << 4);  // Clear NACK flag
                return; //exit from everything, just see diagrams and it's more clear, this is KEY!
            }
        }


        /* Load the next byte to be transmitted into TXDR when empty and ready*/
        I2C1 -> TXDR = data[i];
    }

    /* Wait for the automatic STOP condition to complete (STOPF = 1) */
    while (!(I2C1 -> ISR & (1U << 5))) {}

    /* Clear the STOP detection flag */
    I2C1 -> ICR |= (1U << 5);
}
