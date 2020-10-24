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
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <inttypes.h>
#include <math.h>
#include "atmega328timer.h"
#include "atmega328i2c.h"
#include "function.h"
#include "lcd.h"
#include "keypad.h"
#include "74hc595.h"
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
	FUNC func = FUNCenable();
	LCD0 lcd = LCD0enable(&DDRD,&PIND,&PORTD);
	HC595 shift = HC595enable(&DDRB,&PORTB,3,1,0); //REMEMBER!!!
	/***Local Variable***/
	int32_t b=56;
	int32_t a=38347922;
	int32_t c=0;
	uint8_t i,j;
	i=j=0;
    /* Replace with your application code */
    while (TRUE)
    {
		lcd.gotoxy(0,0);
		lcd.string_size("Welcome",7);	
		lcd.gotoxy(1,0);
		c=a*b;
		lcd.string(func.i32toa(c));
		/***Play around***/
		for(i=0;i<8;i++){
			shift.bit(1);
			shift.out();
			_delay_ms(100);
		}
		for(i=0;i<8;i++){
			shift.bit(0);
			shift.out();
			_delay_ms(100);
		}
		for(i=0;i<8;i++){
			shift.bit(1);
			shift.out();
			_delay_ms(100);
			shift.bit(0);
			shift.out();
			_delay_ms(100);
		}
		/***/
		for(i=7;i;i--){
			shift.bit(1);
			shift.out();
			_delay_ms(100);
		}
		for(i=7;i;i--){
			shift.bit(0);
			shift.out();
			_delay_ms(100);
		}
		for(i=7;i;i--){
			shift.bit(1);
			shift.out();
			_delay_ms(100);
			shift.bit(0);
			shift.out();
			_delay_ms(100);
		}
		/***/
		for(i=0;i<8;i++){
			shift.byte(1<<i);
			_delay_ms(100);
		}
		/******/
		for(j=0;j<10;j++){
			for(i=0;i<8;i++){
				shift.byte(~(1<<i));
				_delay_ms(50);
			}
			for(i=6;i;i--){
				shift.byte(~(1<<i));
				_delay_ms(50);
			}
		}
		for(j=0;j<10;j++){
			for(i=0;i<8;i++){
				shift.byte(~(3<<i));
				_delay_ms(50);
			}
			for(i=6;i;i--){
				shift.byte(~(3<<i));
				_delay_ms(50);
			}
		}
		for(j=0;j<10;j++){
			for(i=0;i<8;i++){
				shift.byte(~(7<<i));
				_delay_ms(50);
			}
			for(i=6;i;i--){
				shift.byte(~(7<<i));
				_delay_ms(50);
			}
		}
		for(j=0;j<10;j++){
			for(i=0;i<8;i++){
				shift.byte(~(0x0F<<i));
				_delay_ms(50);
			}
			for(i=6;i;i--){
				shift.byte(~(0x0F<<i));
				_delay_ms(50);
			}
		}
		for(j=0;j<20;j++){
			for(i=0;i<8;i++){
				shift.byte(~(0x05<<i));
				_delay_ms(50);
			}
			for(i=6;i;i--){
				shift.byte(~(0x05<<i));
				_delay_ms(50);
			}
		}
		/***/
    }
}
/***Definition***/
void PORTINIT(void)
{
	DDRB=0x0B;
	PORTB=0x0B;
}
/***Interrupt***/
/***EOF***/
