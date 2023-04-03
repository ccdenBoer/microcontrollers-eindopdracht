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
void spi_displayDriverInit();
void spi_displayOn();
void spi_displayOff();
void spi_writeLedDisplay( int value );
void wait(int ms);
void spi_writeCharacter(char character, char index);
int spi_getCharacterCode(char character);
void spi_writeText(char *string);
void spi_setText(char str[]);
void spi_moveText(int steps);




#endif	