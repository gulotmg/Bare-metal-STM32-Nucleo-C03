#include "spi.h"


/* macros */
#define TXE (1U << 1)


void gpio_init(void)
{

	RCC -> IOPENR |= (1U<<00); //clock enable GPIOA

	/*using  PA5(SCK), PA6(MISO), PA7(MOSI)* alternate functions*/

	/*PA5*/
	GPIOA->MODER |=(1U<<11); GPIOA->MODER &=~(1U<<10);
	/*PA6*/
	GPIOA->MODER |=(1U<<13); GPIOA->MODER &=~(1U<<12);
	/*PA7*/
	GPIOA->MODER |=(1U<<15); GPIOA->MODER &=~(1U<<14);
	/* PA4 as output pin*/
	GPIOA->MODER &= ~(1U<<9); GPIOA->MODER |= (1U<<8);

	//PA5 alt func AF0
	GPIOA -> AFR[0] &= ~(1U<<20);
	GPIOA -> AFR[0] &= ~(1U<<21);
	GPIOA -> AFR[0] &= ~(1U<<22);
	GPIOA -> AFR[0] &= ~(1U<<23);

	//PA6 alt func AF0

	GPIOA -> AFR[0] &= ~(1U<<24);
	GPIOA -> AFR[0] &= ~(1U<<25);
	GPIOA -> AFR[0] &= ~(1U<<26);
	GPIOA -> AFR[0] &= ~(1U<<27);


	//PA7 alt func AF0

	GPIOA -> AFR[0] &= ~(1U<<28);
	GPIOA -> AFR[0] &= ~(1U<<29);
	GPIOA -> AFR[0] &= ~(1U<<30);
	GPIOA -> AFR[0] &= ~(1U<<31);

}


void spi1_config (void)
{

	//Clock to SPI1

	RCC -> APBENR2 |= (1U << 12);


	//baudrate control (prescaler 1/256)

	SPI1 -> CR1 |= (1U << 3);
	SPI1 -> CR1 |= (1U << 4);
	SPI1 -> CR1 |= (1U << 5);

	//clock phase and polarity


	//setting CPHA = '1' implies no meaning in NSSP bit in CR2 register
	SPI1 -> CR1 |= (1U << 0);
	SPI1 -> CR1 |= (1U << 1);

	//activate full duplex mode

	SPI1 -> CR1 &= ~(1U << 10);

	//set frame format

	SPI1 -> CR1 &= ~(1U << 7);

	//set master mode

	SPI1 -> CR1 |= (1U << 2);

	//set SSM and SSI
	SPI1 -> CR1 |= (1U << 9);
	SPI1 -> CR1 |= (1U << 8);

	//set FIFO reception threshold to a byte
	SPI1 -> CR2 |= (1U<< 12);

	//data size set to a byte
	SPI1 -> CR2 |= (1U << 8);
	SPI1 -> CR2 |= (1U << 9);
	SPI1 -> CR2 |= (1U << 10);
	SPI1 -> CR2 &= ~(1U << 11);

	//ACTIVATE SPI (mandatory at the end)
	SPI1 -> CR1 |= (1U<<6);


}


void spi1_transmitter(uint8_t *data, uint32_t size)
{
    uint32_t i = 0;
    uint8_t temp; //(warning can be ignored)

    while (i < size) {

    	//wait for TX buffer to be empty
        while (!(SPI1 -> SR & (1U << 1))) {}
    	//casting 8 bits<
        *(volatile uint8_t *)&SPI1->DR = data[i];
        i++;


        /* communication in SPI is full-duplex, for every SCK impulse there's an outgoing bit (MOSI) and
         * an incoming bit (MISO). This means that when I send a byte to sensor, I receive a byte that
         * I actually don't need that is stocked in the RxFIFO. This means that if I don't read the FIFO,
         * with 'temp' for every iteration, I'll have an overrun error eventually (second byte sent).
         * For this reason it's important to wait for RX to be not empty and then read. The read operation
         * automatically clears the content of the RxFIFO preventing Overrun issues (clears OVR flag in SR).
         * Failing to do so implies to read some "00000..." values due to OVR issues.
         */


        while (!(SPI1 -> SR & (1U << 0))) {} //wait for RX to be not empty
			temp = *(volatile uint8_t *)&SPI1->DR;
			temp = *(volatile uint8_t *)&SPI1->SR;
	}



    while (!(SPI1 -> SR & (1U << 1))) {} // wait for TXE
    while ((SPI1 -> SR & (1U << 7))) {}  // wait for BSY

    //this allows to make sure communication is finished.

}

void spi1_receiver(uint8_t *data, uint32_t size)
{
    while (size) {

    	/* Dummy data: in order to generate SCK clock signals for the sensor, I need to
    	 * write a dummy byte into the DR register, which pushes it into the TxFIFO.
    	 * Since SPI communication is strictly full-duplex, this clock generation
    	 * forces the sensor to shift its payload back to the MCU via MISO.
    	 * The dummy data shifted out on MOSI is simply ignored by the sensor (SEE mpu6500.c mpu_read).
    	 * Meanwhile, the real data received on MISO is automatically pushed into
    	 * the RxFIFO by the hardware, which I then extract (pop) by reading the DR register.
    	 * This is all valid because DR looks at the oldest (first) window of the RxFIFO.
    	 * The key concept is that with this FIFO architecture the DR is interfaced with the appropriate
    	 * FIFO in accordance to the operation that is ongoing; the DR is just an interface.
    	 */

        *(volatile uint8_t *)&SPI1->DR = 0x00;

        while (!(SPI1 -> SR & (1U << 0))) {} //wait for RX to be not empty

        // REad 8-bit
        *data++ = *(volatile uint8_t *)&SPI1->DR;
        size--;
    }
}


//PA4 is used a CS by putting it high or low directly that's why the output mode


void cs_enable(void)
{
	GPIOA -> ODR &= ~(1U<<4);
}

//active low of course


void cs_disable(void)
{
	GPIOA -> ODR |= (1U<<4);
}
