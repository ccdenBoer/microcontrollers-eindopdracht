/*
 * timer.c
 *
 * Created: 22-3-2023 10:55:57
 *  Author: Coen den Boer & Momin Musa
 */ 

#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
 
 //init a timer resulting in 10 Hz
void timer_init(){
	OCR1A = 31250; // 16-bits compare value of counter 1
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001100; // // Initialize T1: timer, prescaler = 256, compare output disconnected, CTC, RUN
	TIMSK |= (1<<4); // T1 compare match A interrupt enable
}//set the compare value, starting at 10240, allowing the timer to be between 30.5 Hz and 4.76 Hzvoid timer_set_compare_value(int value){	//check for overflow	if(10240 + value > 0xFFFF || 10000 + value < 10000){		OCR1A = 0xFFFF;	} else{		OCR1A = (10240+value);	}		TCNT1 = 0x00;	}//calculate the hz of the current timer (80 Mhz / 256 prescaler / current compare value)int timer_get_hz(){	return 312500/OCR1A;}

