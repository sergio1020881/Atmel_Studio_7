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
#include "atmega128analog.h"
#include "atmega128interrupt.h"
#include "atmega128spi.h"
#include "atmega128timer.h"
#include "atmega128twi.h"
#include "atmega128uart.h"
#include "lcd.h"
#include "function.h"
#include "keypad.h"
#include "lfsm.h"
#include "rotenc.h"
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
	uint8_t option=0; //Menu Jumper
	uint8_t input_tmp=0;
	struct keypadata keypadinput;
	uint16_t output=0;
	uint16_t mask=0;
	uint8_t page=0;
	uint16_t n;
	KEYPAD keypad = KEYPADenable(&DDRE,&PINE,&PORTE);
	LCD0 lcd = LCD0enable(&DDRA,&PINA,&PORTA);
	EEPROM eeprom = EEPROMenable();
	LFSM lfsm = LFSMenable(&eeprom,363);
	FUNC func = FUNCenable();
	//ROTENC potenc = ROTENCenable(0,1);
	/**************************************/
	lfsm.setoutput(&lfsm,0);
	/*** Replace with your application code ***/
	while (True)
	{
		/******/
		keypadinput=keypad.get();
		//n=potenc.rte(&potenc,PINB).num;
		n=PINB;
		/******/
		lcd.gotoxy(0,14);
		string=func.ui16toa(n);
		lcd.string_size(string,5);
		/******/
		switch(option){
			case 0: // Main Menu
				lcd.gotoxy(0,0);
				lcd.string_size("Running ",8);
				string=func.ui16toa(lfsm.getpage(&lfsm));
				lcd.string_size(string,3);
				if(keypadinput.character == 'A'){ // Clear screen and empty buffer
					lcd.clear();
					keypad.flush();
					continue;
				}
				if(keypadinput.character == 'B'){ // Learn
					lcd.clear();
					keypad.flush();
					option=1;
				}
				if(keypadinput.character == 'C'){ // Delete or remove
					lcd.clear();
					keypad.flush();
					option=6;
				}
				lfsm.read(&lfsm,n); // Getting answer
				break;
			case 1: // New Entry Menu
				// Menu
				lcd.gotoxy(0,0);
				lcd.string_size("Enter: ",12);
				lcd.gotoxy(1,0);
				lcd.string_size("In   Out  Mask Page",20);
				option=2;
				break;
			case 2: // First entry [input]
				lcd.gotoxy(0,7);
				lcd.string_size(keypadinput.string,4);
				if(keypadinput.character == 'D'){
					input_tmp=func.strToInt(keypadinput.string);
					lcd.gotoxy(2,0);
					string=func.ui16toa(input_tmp);
					lcd.string(string);
					keypad.flush();
					option=3;
				}
				if(keypadinput.character == 'C'){
					keypad.flush();
					option=2;
				}
				break;
			case 3: // Second Entry [output]
				lcd.gotoxy(0,7);
				lcd.string_size(keypadinput.string,4);
				if(keypadinput.character == 'D'){
					output=func.strToInt(keypadinput.string);
					lcd.gotoxy(2,5);
					string=func.ui16toa(output);
					lcd.string(string);
					keypad.flush();
					option=4;
				}
				if(keypadinput.character == 'C'){
					keypad.flush();
					option=3;
				}
				break;
			case 4: // Third Entry [mask]
				lcd.gotoxy(0,7);
				lcd.string_size(keypadinput.string,4);
				if(keypadinput.character == 'D'){
					mask=func.strToInt(keypadinput.string);
					lcd.gotoxy(2,10);
					string=func.ui16toa(mask);
					lcd.string(string);
					keypad.flush();
					option=5;
				}
				if(keypadinput.character == 'C'){
					keypad.flush();
					option=4;
				}
				break;
			case 5: // Fourth entry [page] and upload with reply
				lcd.gotoxy(0,7);
				lcd.string_size(keypadinput.string,4);
				if(keypadinput.character == 'D'){ // D is the enter key on the keyboard
					page=func.strToInt(keypadinput.string);
					lcd.gotoxy(2,15);
					string=func.ui16toa(page);
					lcd.string(string);
					switch(lfsm.learn(&lfsm,input_tmp,output,mask,page)){
						case 0: // Not used
							lcd.gotoxy(3,12);
							lcd.string_size("disabled",7);
							break;
						case 1: // already programmed
							lcd.gotoxy(3,12);
							lcd.string_size("exists",7);
							break;
						case 2: // new entry edded
							lcd.gotoxy(3,12);
							lcd.string_size("upload",7);
							break;
						case 3: // new entry added
							lcd.gotoxy(3,12);
							lcd.string_size("upload",7);
							break;
						case 4: // eeprom is full reject entry
							lcd.gotoxy(3,12);
							lcd.string_size("full",7);
							break;
						default:
							break;
					}//End switch
				/******/
				//lfsm.read(&lfsm,input_tmp);
				keypad.flush();
				option=0;
				}//End if
				if(keypadinput.character == 'C'){ // re-enter value [page] 
					keypad.flush();
					option=5;
				}
				break;
			case 6: // Delete or Remove or No action Menu
				lcd.gotoxy(1,0);
				lcd.string_size("Delete - 1",10);
				lcd.gotoxy(2,0);
				lcd.string_size("Remove - 2 No - 3",17);
				option=7;
				break;
			case 7: // Delete or Remove or No action choice
				if(keypadinput.character == '1'){ // Delete
					lcd.gotoxy(1,0);
					lcd.hspace(10);
					lcd.gotoxy(2,0);
					lcd.hspace(17);
					lfsm.deleteall(&lfsm);
					lfsm.setpage(&lfsm,0);
					//lfsm.setoutput(&lfsm,0X00);
					keypad.flush();
					lcd.gotoxy(3,12);
					lcd.string_size("deleted",7);
					option=0;
				}else if(keypadinput.character == '2'){ // Remove
					lcd.gotoxy(1,0);
					lcd.hspace(10);
					lcd.gotoxy(2,0);
					lcd.hspace(17);
					keypad.flush();
					option=8;
				}else if(keypadinput.character == '3'){ // No action
					lcd.gotoxy(1,0);
					lcd.hspace(10);
					lcd.gotoxy(2,0);
					lcd.hspace(17);
					keypad.flush();
					option=0;
				}
				break;
			case 8: // Remove Entry Data
				lcd.gotoxy(0,0);
				lcd.string_size("Enter: ",12);
				lcd.gotoxy(1,0);
				lcd.string_size("Input",20);
				option=9;
				break;
			case 9: // Remove Entry from EEprom
				lcd.gotoxy(0,7);
				lcd.string_size(keypadinput.string,4);
				if(keypadinput.character == 'D'){ // D is the enter key on the keyboard
					input_tmp=func.strToInt(keypadinput.string);
					lcd.gotoxy(2,0);
					string=func.ui16toa(input_tmp);
					lcd.string(string);
					switch(lfsm.remove(&lfsm,input_tmp)){ // feedback reply
						case 0:
							lcd.gotoxy(3,12);
							lcd.string_size("No entry",7);
							break;
						case 1:
							lcd.gotoxy(3,12);
							lcd.string_size("New Entry",7);
							break;
						case 2:
							lcd.gotoxy(3,12);
							lcd.string_size("global logic",7);
							break;
						case 3:
							lcd.gotoxy(3,12);
							lcd.string_size("local logic",7);
							break;
						case 4:
							lcd.gotoxy(3,12);
							lcd.string_size("local logic",7);
							break;
						case 41:
							lcd.gotoxy(3,12);
							lcd.string_size("local above",7);
							break;
						case 42:
							lcd.gotoxy(3,12);
							lcd.string_size("local bellow",7);
							break;
						default:
							break;
					}//End switch
					/******/
					//lfsm.read(&lfsm,input_tmp);
					
					keypad.flush();
					option=0;
				}//End if
				if(keypadinput.character == 'C'){ // Repeat Remove Data Entry
					keypad.flush();
					option=8;
				}
				break;
			default:
				break;
		}//End switch
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
