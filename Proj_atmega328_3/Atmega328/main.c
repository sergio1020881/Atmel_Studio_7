/***********************************************************
Hardware: Atmega328.c
Author: Sergio Santos
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
#include "lcd.h"
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
	/***Local Variable***/
	FUNC func = FUNCenable();
	LCD0 lcd = LCD0enable(&DDRD,&PIND,&PORTD);
	//HC595 shift = HC595enable(&DDRB,&PORTB,3,1,0); //REMEMBER!!!
	//UART uart = UARTenable(103,8,1,NONE);
	//char* uartreceive;
	int32_t a,b,c,d;
    /***Replace with your application code***/
    while (TRUE)
    {
		b=999999999;
		c=345345030;
		a=b/c;
		d=b%c;
		lcd.gotoxy(0,0);
		lcd.string_size("Welcome",7);
		lcd.gotoxy(1,0);
		lcd.string(func.i32toa(a));lcd.hspace(3);lcd.string(func.i32toa(d));
		
		
		
		
		
	}
}
/***Definition***/
void PORTINIT(void)
{
	
}
/***Interrupt***/
/***EOF***/
