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

int main(void)
{
	DDRC = 0xFF;
	PORTC = 0x00;
	
	init_4bits_mode();
	lcd_clear();
	lcd_write_string("yoo");
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

