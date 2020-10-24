/***********************************************************
Hardware: Atmega328.c
Author : Sergio Santos
	<sergio.salazar.santos@gmail.com>
Created: 04/10/2020 16:37:53
Comment:
	When multiplying big numbers always use a third variable 
	to store the result and be aware to not overflow, because
	32 bit -> 4294967296 - 1, if unsigned otherwise 
	2147483648 - 1 for signed. Thats the best for 8 bit MCU.
	Stable
 **********************************************************/
#define F_CPU 16000000UL
/***library***/
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <inttypes.h>
#include <math.h>
#include "atmega328timer.h"
//#include "atmega328i2c.h"
#include "function.h"
#include "uart.h"
/***Consytant and Macros***/
#define TRUE 1
#define ZERO 0
/***Global File Variables***/
/***Header***/
void PORTINIT(void);
/***MAIN***MAIN****/
int main(void)
{
	/***Preamble***/
	//FUNC func = FUNCenable();
	UART uart = UARTenable(103,8,1,NONE);
	/***Local Variable***/
	//char* uartreceive;
    /* Replace with your application code */
    while (TRUE)
    {
		if(!(PIND & 0x04))
		{
			uart.putc('>');
			uart.puts("Welcome to uart com threw ftdi ! \r \n");
			uart.puts("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA ! \r \n");
		}
		PORTB|=0x02;
		_delay_ms(100);
		PORTB&=~(0x02);
		_delay_ms(100);
    }
}
/***Definition***/
void PORTINIT(void)

{
	DDRD&=~(0x04);
	PORTD|=0x04;
	DDRB|=0x02;
	PORTB=0X02;
}
/***Interrupt***/
/***EOF***/
