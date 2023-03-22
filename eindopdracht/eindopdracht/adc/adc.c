/*
 * adc.c
 *
 * Created: 22-3-2023 10:52:14
 *  Author: Coen den Boer & Momin Musa
 */ 

#include <avr/io.h>
#include "adc.h"


void adc_init(){
	ADMUX = 0b01100001;			// AREF=VCC, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b11100110;		// ADC-enable, no interrupt, start, free running, division by 64
}

