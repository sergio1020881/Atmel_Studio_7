/************************************************************************
Title: ATMEGA 128
Author: Sergio Manuel Santos
	<sergio.salazar.santos@gmail.com>
File: main.c 2019/02/16 14:00:00
Software: Atmel Studio 7 (Version: 7.0.1931)
Hardware: Atmega128 by ETT ET-BASE
	-PORTA: lcd display 2x16 or 4x20
	-PORTE: keypad
	-PORTG: rotary potentiometer
	-16Mhz
COMMENT:
	stable
************************************************************************/
/***preamble inic***/
#define F_CPU 16000000UL
/***Library***/
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "Atmega128API.h"
#include "function.h"
#include "clock.h"
#include "lcd.h"
#include "keypad.h"
#include "eeprom.h"
#include "lfsm.h"
/***Constant & Macro***/
#define TRUE 1
#define FALSE 0
#define GI 7
/***Global File Variable***/
/***Header***/
void PORTINIT();
/***MAIN***MAIN***/
int main(void)
{
	PORTINIT();
	uint8_t potenc=0;
	uint16_t priorpotenc=3;
	uint16_t incdec=0;
	char string[20]="\0";
	/***INICIALIZE OBJECTS***/
	FUNC function = FUNCenable();
	LCD0 lcd0 = LCD0enable(&DDRA,&PINA,&PORTA);
	KEYPAD keypad = KEYPADenable(&DDRE,&PINE,&PORTE);
	while(TRUE){
		lcd0.reboot();
		potenc=PING&3;
		//TODO:: Please write your application code
		lcd0.gotoxy(0,0);
		lcd0.string_size("Boot Up",7);
		lcd0.gotoxy(1,2);
		lcd0.string_size("Run",3);
		lcd0.gotoxy(2,0);
		lcd0.string(keypad.get().string);
		lcd0.gotoxy(3,0);
		if(priorpotenc==3){
			if(function.hl(priorpotenc,potenc)&1)
				incdec--;
			else if(function.hl(priorpotenc,potenc)&2)
				incdec++;
		}
		priorpotenc=potenc;
		function.ui16toa(incdec,string);
		lcd0.string_size(string,10);
		if(keypad.get().character=='C'){
			keypad.flush();
			lcd0.clear();
		}
	}
}
/***Procedure & Function***/
void PORTINIT()
{
	DDRG|=0B00000100;
	PORTG|=0B00000011;
	SREG|=(1<<GI);
}
/***Interrupt***/
/***EOF***/
