/*
 * timer.c
 *
 * Created: 22-3-2023 10:55:57
 *  Author: Coen den Boer & Momin Musa
 */ 

#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//Timer1 Prescaler = 256; Preload = 31249; Actual Interrupt Time = 1
 
void timer_init(){
	OCR1A = 31250; // 16-bits compare value of counter 1
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001100; // // Initialize T1: timer, prescaler = 256, compare output disconnected, CTC, RUN
	TIMSK |= (1<<4); // T1 compare match A interrupt enable
	sei();
}

