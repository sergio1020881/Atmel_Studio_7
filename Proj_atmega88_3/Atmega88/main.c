/******************************************
	Atmega88.c
Created: 08/10/2020 13:13:01
Author: Sergio Santos
	<sergio.salazar.santos@gmail.com>
*******************************************/
/***preamble inic***/
#define F_CPU 8000000UL
/*
** library
*/
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>
#include "74hc595.h"
/***Constant and Macros***/
#define TRUE 1
#define ZERO 0
/***Global Variable***/
/***Procedure & Function Def***/
/***MAINMAIN***/
int main(void)
{
	uint8_t i;
	//Local Variables
	HC595 shift = HC595enable(&DDRD,&PORTD,4,5,7);
    /* Replace with your application code */
    while (TRUE)
    {
		for(i=0;i<8;i++){
			_delay_ms(100);
			shift.bit(1);
			shift.out();
		}
		for(i=0;i<8;i++){
			_delay_ms(100);
			shift.bit(0);
			shift.out();
		}
		_delay_ms(1000);
		shift.byte(3);
		_delay_ms(1000);
		shift.byte(12);
		_delay_ms(1000);
		shift.byte(48);
		_delay_ms(1000);
		shift.byte(192);
		_delay_ms(1000);
		shift.byte(48);
		_delay_ms(1000);
		shift.byte(12);
		_delay_ms(1000);
		shift.byte(3);
		_delay_ms(1000);
		shift.byte(24);
		_delay_ms(1000);
		for(i=0;i<255;i++){
			shift.byte(i);
			_delay_ms(50);
		}
		
		
		
		
    }
}
/***EOF***/
