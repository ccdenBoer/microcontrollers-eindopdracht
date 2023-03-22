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
bool right_direction = true;

int number = 0;

ISR( TIMER1_COMPA_vect ) {
	writeLedDisplay(msCount);
	number = ADCH << 2;
	number |= ADCL >> 6;
	timer_set_compare_value(20*number);
	
	if(right_direction){
		msCount++;
		//move right
	} else {
		msCount--;
		//move left
	}
	
	
}

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
	displayDriverInit();            // Initialize display chip
	// clear display (all zero's)
	for (char i =1; i<=4; i++)
	{
		spi_slaveSelect(0); 		// Select display chip
		spi_write(i);  				// 	digit adress: (digit place)
		spi_write(0);				// 	digit value: 0
		spi_slaveDeSelect(0);		// Deselect display chip
	}
		
	//adc
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;
	//DDRD = 0xFF;
	adc_init();
	
	number = ADCH << 2;
	number |= ADCL >> 6;
	
	//timer
	//DDRD = 0xFF;
	timer_init();
	number = ADCH << 2;
	number |= ADCL >> 6;
	timer_set_compare_value(6*number);
	
	//interupt
	// Init I/O
	DDRE = 0x01;			// PORTE 0 input	

	// Init Interrupt hardware
	EICRA |= 0x03;			// INT0 rising edge
	EIMSK |= 0x01;			// Enable INT0
	
	sei();
	
    while (1) 
    {

		PORTA = ADCH;
		//PORTD = ADCL;
		
		lcd_clear();
		lcd_write_integer(msCount);
		lcd_write_string("-");
		lcd_write_integer(number);

		wait(1000);
    }
}

