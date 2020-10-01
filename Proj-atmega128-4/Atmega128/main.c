/*** 
File: main.c
Author: Sergio Santos
	<sergio.salazar.santos@gmail.com>
Hardware: Atmega
	-PORTA : LCD Display 4X20
	-PORTB : Buttons
	-PORTC : Relay Board
	-PORTE : Keypad 4X4
Date:
	30/09/2020
Comment:
	LFSM
***/
/***FCPU***/
#define F_CPU 16000000UL
/*
** library
*/
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
/*
 ** Constant and Macro
 */
#define True 1
/*
 ** Global Variable
 */
char* string=NULL;
/*
 ** Function Definition
 */
void PORTINIT(void);
/***MAIN***/
int main(void)
{
	PORTINIT();
	uint8_t n;
	//KEYPAD keypad = KEYPADenable(&DDRE,&PINE,&PORTE);
	LCD0 lcd = LCD0enable(&DDRA,&PINA,&PORTA);
	EEPROM eeprom = EEPROMenable();
	LFSM lfsm = LFSMenable(&eeprom,571);
	FUNC func = FUNCenable();
	/**************************************/
	lfsm.setoutput(&lfsm,0);
	/*** Replace with your application code ***/
	while (True)
	{
		n=PINB;
		/******/
		lcd.gotoxy(0,16);
		string=func.ui16toa(n);
		lcd.string_size(string,4);
		lfsm.read(&lfsm,n);
		/***DISPLAY***/
		lcd.gotoxy(3,0);
		lcd.string_size("Output ",7);
		string=func.ui16toa(lfsm.getoutput(&lfsm));
		lcd.string_size(string,4);
		//lcd.hspace(2);
		//string=func.ui16toa(lfsm.getstatus(&lfsm));
		//lcd.string_size(string,4);
		PORTC=lfsm.getoutput(&lfsm);
		
	}//End while
}//End main
/***Procedure and Function***/
void PORTINIT(void){
	DDRC=0XFF;
	PORTC=0XFF;
	DDRB=0X00;
	PORTB=0XFF;
}
/***Interrupt***/
/***Comment
The more one programs the more precise our logical thinking becomes
absent from error.
***/
/***EOF***/
