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

volatile int msCount = 0;
static bool right_direction = true;

static int number = 0;

ISR( TIMER1_COMPA_vect ) {
	//read out the 10 bit value
	number = ADCH << 2;
	number |= ADCL >> 6;
	
	//10.240 + 54 * 1024(max 10 bit value) = 65536(max value of an 16 bit integer)
	timer_set_compare_value(54*number);
	
	//move the text on the 7-seg display
	if(right_direction){
		spi_moveText(1);
	} else {
		spi_moveText(-1);
	}
	
	
}

//react to button pres to change the direction the 7-seg display scrolls
ISR( INT0_vect ) {
    if(right_direction){
		right_direction = false;
	} else {
		right_direction = true;
	}
}

int main(void)
{
	//lcd
	DDRC = 0xFF;
	PORTC = 0x00;
	
	init_4bits_mode();
	lcd_clear();
	
	//spi
	// inilialize
	DDRB=0x01;					  	// Set PB0 pin as output for display select
	spi_masterInit();              	// Initialize spi module
	spi_displayDriverInit();            // Initialize display chip
	// clear display (all zero's)
	for (char i =1; i<=4; i++)
	{
		spi_slaveSelect(0); 		// Select display chip
		spi_write(i);  				// 	digit adress: (digit place)
		spi_write(0);				// 	digit value: 0
		spi_slaveDeSelect(0);		// Deselect display chip
	}
	
	char *text = (char *) malloc(sizeof(char) * 25);
	snprintf(text, 25, "abcdef ");
	spi_setText(text);
		
	//adc initialization
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;				// set PORTA for output
	adc_init();
	
	//read the 10 bits from the adc into number
	number = ADCH << 2;
	number |= ADCL >> 6;

	//timer initialization
	timer_init();
	number = ADCH << 2;
	number |= ADCL >> 6;
	timer_set_compare_value(54*number);
	
	//interupt initialization
	DDRE = 0x01;			// PORTE 0 input	

	// Init Interrupt hardware
	EICRA |= 0x03;			// INT0 rising edge
	EIMSK |= 0x01;			// Enable INT0
	
	//enable global interupts
	sei();
	
    while (1) 
    {

		PORTA = ADCH;
		
		//write the full string and speed to the lcd, "<text> - <speed> hz"
		lcd_clear();
		lcd_write_string(text);
		wait(3);
		lcd_write_string(" - ");
		wait(3);
		int time = timer_get_hz();
		lcd_write_integer((time - time % 10)/10);
		wait(3);
		lcd_write_string(".");
		wait(3);
		lcd_write_integer(time % 10);
		wait(3);
		lcd_write_string(" Hz");
		
		wait(1000);
    }
}

