/*
 * eindopdracht.c
 *
 * Created: 22-3-2023 10:15:39
 * Author : Coen den Boer & Momin Musa
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "lcd/lcd.h"
#include "spi/spi.h"
#include "adc/adc.h"
#include "timer/timer.h"

#ifndef BIT
#define BIT(x)		( 1<<x )
#endif

int main(void)
{
	//lcd
	DDRC = 0xFF;
	PORTC = 0x00;
	
	init_4bits_mode();
	lcd_clear();
	lcd_write_string("yoo");
	
	//spi
	// inilialize
	DDRB=0x01;					  	// Set PB0 pin as output for display select
	spi_masterInit();              	// Initialize spi module
	displayDriverInit();            // Initialize display chip
	// clear display (all zero's)
	for (char i =1; i<=4; i++)
	{
		spi_slaveSelect(0); 		// Select display chip
		spi_write(i);  				// 	digit adress: (digit place)
		spi_write(0);				// 	digit value: 0
		spi_slaveDeSelect(0);		// Deselect display chip
	}
	wait(1000);
	// write 4-digit data
	writeLedDisplay(-1);
	writeLedDisplay(-12);
	writeLedDisplay(-756);
	writeLedDisplay(-1582);
	writeLedDisplay(8);
	writeLedDisplay(12);
	writeLedDisplay(852);
	writeLedDisplay(1564);
	
    while (1) 
    {
    }
}

