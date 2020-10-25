/****************************************************************
File: main.c
Author: Sergio Santos
	<sergio.salazar.santos@gmail.com>
Hardware: Atmega 128
	-PORTA : LCD Display 4X20
	-PORTB : Buttons or Potentiometer encoder
	-PORTC : Relay Board
	-PORTE : Keypad 4X4
Date:
	05/10/2020
Comment:
	LFSM & ROTENC
****************************************************************/
/***FCPU***/
#define F_CPU 16000000UL
/***library***/
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "eeprom.h"
#include "Atmega128API.h"
#include "lcd.h"
#include "function.h"
#include "keypad.h"
#include "lfsm.h"
#include "rotenc.h"
/***Constant & Macro***/
#define True 1
/***Global File Variable***/
char* string=NULL;
/***Header***/
void PORTINIT(void);
/***MAIN***MAIN***/
int main(void)
{	
	PORTINIT();
	uint8_t m;
	//KEYPAD keypad = KEYPADenable(&DDRE,&PINE,&PORTE);
	LCD0 lcd = LCD0enable(&DDRA,&PINA,&PORTA);
	EEPROM eeprom = EEPROMenable();
	LFSM lfsm = LFSMenable(&eeprom,571);
	FUNC func = FUNCenable();
	ROTENC potenc = ROTENCenable(3,7);
	/**************************************/
	lfsm.setoutput(&lfsm,0);
	/*** Replace with your application code ***/
	while (True)
	{
		//m=RotEnc.rte(&RotEnc,PINB).num;
		m=potenc.rte(&potenc,PINB).num;
		/******/
		lcd.gotoxy(0,0);
		string=func.ui16toa(m);
		lcd.string_size(string,4);
		lfsm.read(&lfsm,m);
		/***DISPLAY***/
		lcd.gotoxy(1,0);
		lcd.string_size("Output ",7);
		string=func.ui16toa(lfsm.getoutput(&lfsm));
		lcd.string_size(string,4);
		PORTC=lfsm.getoutput(&lfsm);
		
		lcd.gotoxy(2,0);
		lcd.string_size("PotEnc ",7);
		string=func.ui16toa(m);
		lcd.string_size(string,4);	
	}
}
/***Procedure & Function***/
void PORTINIT(void)
{
	DDRC=0XFF;
	PORTC=0XFF;
	DDRB=0X77; // rotenc pb3 e pb7 input resto output.
	PORTB=0X88; //rotenc pb3 e pb7 pull up resistor o resto output a zero.
	//DDRB=0X00; // butoes tudo entrada
	//PORTB=0XFF; // butoes tudo com pullups
	
}
/***Interrupt***/
/***Comment
The more one programs the more precise our logical thinking becomes
absent from error.
***/
/***EOF***/
