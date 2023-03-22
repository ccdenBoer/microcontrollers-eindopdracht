/*
 * spi.c
 *
 * Created: 22/03/2023 10:40
 * Author : Coen den Boer & Momin Musa
 */ 

#ifndef _SPI_H
#define _SPI_H

#ifndef BIT
#define BIT(x)		( 1<<x )
#endif

void spi_masterInit(void);
void spi_write( unsigned char data );
char spi_writeRead( unsigned char data );
void spi_slaveSelect(unsigned char chipNumber);
void spi_slaveDeSelect(unsigned char chipNumber);
void spi_writeWord ( unsigned char adress, unsigned char data );
void displayDriverInit();
void displayOn();
void displayOff();
void writeLedDisplay( int value );
void wait(int ms);
void writeCharacterToDisplay(char character, int index);
int getCharacterCode(char character);




#endif	