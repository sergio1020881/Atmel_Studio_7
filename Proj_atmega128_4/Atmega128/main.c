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
#define PinChnA 0
#define PinChnB 1
/*
 ** Global Variable
 */
char* string=NULL;
struct Rtnc{
	uint8_t pchn;
	uint8_t chn;
	uint8_t num;
	struct Rtnc (*rte)(struct Rtnc *self, uint8_t data);
	} RotEnc;
/*
 ** Function Definition
 */
void PORTINIT(void);
struct Rtnc RotEnc_rte(struct Rtnc *self, uint8_t data);
/***MAIN***/
int main(void)
{
	RotEnc.pchn=(1<<PinChnB)|(1<<PinChnA);
	RotEnc.chn=(1<<PinChnB)|(1<<PinChnA);
	RotEnc.rte=RotEnc_rte;
	PORTINIT();
	uint8_t m;
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
		m=RotEnc.rte(&RotEnc,PINB).num;
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
		lcd.string_size("RotEnc ",7);
		string=func.ui16toa(m);
		lcd.string_size(string,4);
		
	}//End while
}//End main
/***Procedure and Function***/
void PORTINIT(void)
{
	DDRC=0XFF;
	PORTC=0XFF;
	DDRB=0X00;
	PORTB=0XFF;
}
struct Rtnc RotEnc_rte(struct Rtnc *self, uint8_t data)
{
	uint8_t hl;
	self->chn=data & ((1<<PinChnB)|(1<<PinChnA));
	hl=self->chn ^ self->pchn;
	hl&=self->pchn;
	if(self->pchn != self->chn){
		if((self->pchn == ((1<<PinChnB)|(1<<PinChnA))) && (hl & (1<<PinChnA)))
			self->num++;
		if((self->pchn == ((1<<PinChnB)|(1<<PinChnA))) && (hl & (1<<PinChnB)))
			self->num--;
	}
	self->pchn=self->chn;
	return *self;
}
/***Interrupt***/
/***Comment
The more one programs the more precise our logical thinking becomes
absent from error.
***/
/***EOF***/
