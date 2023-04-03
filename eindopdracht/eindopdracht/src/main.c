/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();

	/* Insert application code here, after the board has been initialized. */
}
spi_write(0x03);   				// 	-> 1 = Display digits 0..3

// clear display (all zero's)
for (char i =1; i<=4; i++)

// write 4-digit data
for (char i =1; i<=4; i++)
A/B1.2
spi_write(0x0F);    			//  -> Level 4 (in range [1..F])
A/C
void spi_writeWord ( unsigned char adress, unsigned char data ) {
	spi_slaveSelect(0);
	spi_write(adress);
	spi_write(data);
	spi_slaveDeSelect(0);
	
}
void displayDriverInit()
{
	spi_writeWord (0x09, 0xFF);
	spi_writeWord (0x0A, 0x04);
	spi_writeWord (0x0B, 0x03);
	spi_writeWord (0x0C, 0x01);

}
B.1
void writeLedDisplay( int value ) {
	if(value < 0) {
		value = value*-1;
	}
	int i, divisor = 10000;
	int digit = 0;
	for (i = 5; i > 0; i--) {
		digit = value / divisor;
		value %= divisor;
		divisor /= 10;
		spi_writeWord(i, digit);
		wait(1000);
	}

}
B.2
void writeLedDisplay( int value ) {
	int min_pos = -1;
	int pos_value = value;
	if(value < 0) {
		pos_value *= -1;
		min_pos = 4;
	}
	
	if((value > -10) && (value < 0)){
		min_pos = 2;
		} else if((value > -100) && (value < 0)){
		min_pos = 3;
	}
	
	int i,divisor = 10000;
	int digit = 0;
	for (i = 5; i > 0; i--) {
		digit = pos_value / divisor;
		pos_value %= divisor;
		divisor /= 10;
		spi_writeWord(i, digit);
		wait(10);
	}
	if(min_pos>-1){
		spi_writeWord(min_pos, 10);
	}
	wait(1000);
	
}
