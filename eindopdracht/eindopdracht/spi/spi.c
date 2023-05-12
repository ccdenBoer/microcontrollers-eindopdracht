/* spi Serial Peripheral Interface
 *	 7 segments display
 *  Master
 *	WvdE
 * 24 febr 2011
 */
/*
 * Project name		: Demo5_5 : spi - 7 segments display
 * Author			: Avans-TI, WvdE, JW
 * Revision History	: 20110228: - initial release;
 * Description		: This program sends data to 1 4-digit display with spi
 * Test configuration:
     MCU:             ATmega128
     Dev.Board:       BIGAVR6
     Oscillator:      External Clock 08.0000 MHz
     Ext. Modules:    Serial 7-seg display
     SW:              AVR-GCC
 * NOTES			: Turn ON switch 15, PB1/PB2/PB3 to MISO/MOSI/SCK
*/

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "spi.h"
#include <ctype.h>
#include <string.h>

#define DDR_SPI		DDRB					// spi Data direction register
#define PORT_SPI	PORTB					// spi Output register
#define SPI_SCK		1						// PB1: spi Pin System Clock
#define SPI_MOSI	2						// PB2: spi Pin MOSI
#define SPI_MISO	3						// PB3: spi Pin MISO
#define SPI_SS		0						// PB0: spi Pin Slave Select

static int position = 0;
static char *text = NULL;
// wait(): busy waiting for 'ms' millisecond
// used library: util/delay.h
void wait(int ms)
{
	for (int i=0; i<ms; i++)
		{
			_delay_ms(1);
		}
}

void spi_masterInit(void)
{
	DDR_SPI = 0xff;							// All pins output: MOSI, SCK, SS, SS_display as output
	DDR_SPI &= ~BIT(SPI_MISO);				// 	except: MISO input
	PORT_SPI |= BIT(SPI_SS);				// SS_ADC == 1: deselect slave
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);	// or: SPCR = 0b11010010;
											// Enable spi, MasterMode, Clock rate fck/64, bitrate=125kHz
											// Mode = 0: CPOL=0, CPPH=0;
}


// Write a byte from master to slave
void spi_write( unsigned char data )				
{
	SPDR = data;							// Load byte to Data register --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); 			// Wait for transmission complete 
}

// Write a byte from master to slave and read a byte from slave
// nice to have; not used here
char spi_writeRead( unsigned char data )
{
	SPDR = data;							// Load byte to Data register --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); 			// Wait for transmission complete 
	data = SPDR;							// New received data (eventually, MISO) in SPDR
	return data;							// Return received byte
}

// Select device on pinnumer PORTB
void spi_slaveSelect(unsigned char chipNumber)
{
	PORTB &= ~BIT(chipNumber);
}

// Deselect device on pinnumer PORTB
void spi_slaveDeSelect(unsigned char chipNumber)
{

	PORTB |= BIT(chipNumber);
}


// Initialize the driver chip (type MAX 7219)
void spi_writeWord ( unsigned char adress, unsigned char data ) {
	spi_slaveSelect(0);
	spi_write(adress);
	spi_write(data);
	spi_slaveDeSelect(0);
}
void spi_displayDriverInit() 
{
	spi_writeWord (0x09, 0xFF);
	spi_writeWord (0x0A, 0x04);
	spi_writeWord (0x0B, 0x03);
	spi_writeWord (0x0C, 0x01);

}

// Set display on ('normal operation')
void spi_displayOn() 
{
  	spi_slaveSelect(0);				// Select display chip
  	spi_write(0x0C); 				// Register 0B: Shutdown register
  	spi_write(0x01); 				// 	-> 1 = Normal operation
  	spi_slaveDeSelect(0);			// Deselect display chip
}

// Set display off ('shut down')
void spi_displayOff() 
{
  	spi_slaveSelect(0);				// Select display chip
  	spi_write(0x0C); 				// Register 0B: Shutdown register
  	spi_write(0x00); 				// 	-> 1 = Normal operation
  	spi_slaveDeSelect(0);			// Deselect display chip
}

void spi_writeCharacter(char character, char index) {
	spi_writeWord(0x9, 0);
	spi_writeWord(index, spi_getCharacterCode(character));
}
void spi_setText(char *str) {
	text = str;
	spi_writeText(str);
}

void spi_writeText(char *string) {
	while(strlen(string) < 4){
		strcat(string, " ");
	}
	
	int j = 4;
	for (int i = 0; i < 4; i++, j--) {
		spi_writeCharacter(string[i], j);
	}
}

void spi_moveText(int steps) {
	int len = strlen(text);
	position+=steps;
	if(position < 0){
		position = len + steps;
	}
	position%=len;
	char newText[] = {text[(position)%(len)],text[(1+position)%(len)],text[(2+position)%(len)],text[(3+position)%(len)], 0} ;
	spi_writeText(newText);
}

int spi_getCharacterCode(char character) {
	switch (toupper(character)) {
		case 'A':
			return 0x77;
		case 'B':
			return 0x1F;
		case 'C':
			return 0x4E;
		case 'D':
			return 0x3D;
		case 'E':
			return 0x4F;
		case 'F':
			return 0x47;
		case 'G':
			return 0x5E;
		case 'H':
			return 0x37;
		case 'I':
			return 0x06;
		case 'J':
			return 0x3C;
		case 'L':
			return 0x0E;
		case 'N':
			return 0x15;
		case 'O':
			return 0x7E;
		case 'P':
			return 0x67;
		case 'Q':
			return 0x73;
		case 'R':
			return 0x05;
		case 'S':
			return 0x5B;
		case 'T':
			return 0x0F;
		case 'U':
			return 0x3E;
		case 'Y':
			return 0x3B;
		default:
			return 0x00;  // return 0x00 for any other character
	}
}
void spi_writeLedDisplay( int value ) {
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
		wait(1);
    }
	if(min_pos>-1){
		spi_writeWord(min_pos, 10);
	}
	wait(1);
	
}


