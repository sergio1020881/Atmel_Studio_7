/***************************************************************************
	Wavegen
Author: Sergio Santos email: sergio.salazar.santos@gmail.com
Created: 20/03/2018 18:35:21
Hardware: Atmega324A
	-PORTA - keypad
	-PORTC - LCD
	-PORTD5 - Oscillator output
Comment:
	Stable
 ***************************************************************************/
//Good practice initialize
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "atmega324analog.h"
#include "atmega324preamble.h"
#include "atmega324timer.h"
#include "keypad.h"
#include "lcd.h"
#include "lfsm.h"
#include "function.h"
#include "eeprom.h"
#include "uart.h"
//Constant & macros
#define True 1
#define False 0
//Global var
uint16_t N_off=10;
uint16_t N_on=8000;
volatile uint16_t counter=0;
//Prototype header
void PORTINIT(void);
/******/
int main(void)
{
	PORTINIT();
	//Local var
	//uint8_t inc=0;
	uint16_t compare;
	uint16_t tcompare;
	uint16_t prescaler;
	uint8_t steprescaler=0;
	uint16_t tN_off;
	uint16_t tN_on;
	char input;
	char value[6];
	//Initialize objects
	ATMEGA324 atmega324 = ATMEGA324enable();
	TIMER_COUNTER1 tim1 = TIMER_COUNTER1enable(4,3);//4,0
	KEYPAD keypad = KEYPADenable(&DDRA,&PINA,&PORTA);
	LCD0 lcd0 = LCD0enable(&DDRC,&PINC,&PORTC);
	FUNC func = FUNCenable();
	EEPROM eeprom = EEPROMenable();
    /* Init Values */
	lcd0.gotoxy(0,0);
	lcd0.string_size("Bom dia !",12);
	lcd0.BF();
	tcompare=compare=eeprom.read_word((uint16_t*)0);
	prescaler=eeprom.read_word((uint16_t*)4);
	tN_off=N_off=eeprom.read_word((uint16_t*)8);
	tN_on=N_on=eeprom.read_word((uint16_t*)12);
	switch(prescaler){
		case 1:
			steprescaler=1;
			break;
		case 8:
			steprescaler=2;
			break;
		case 64:
			steprescaler=3;
			break;
		case 256:
			steprescaler=4;
			break;
		case 1024:
			steprescaler=0;
			break;
		default:
			break;
	}//endswitch	
	tim1.compoutmodeA(1);
	tim1.compareA(compare);
	tim1.start(prescaler);
    while (True)
    {
		atmega324.ioread();
		input=keypad.getkey();
		if(input){
			lcd0.BF();
			lcd0.gotoxy(1,0);
			lcd0.string("Key: ");
			lcd0.putch(input);
			lcd0.hspace(3);
			lcd0.BF();
			lcd0.gotoxy(0,0);
			lcd0.string_size("Running !",12);
			lcd0.BF();
			//DEFAULT
			if(input == 'D'){
				tcompare=compare=2048;
				prescaler=1024;
				steprescaler=0;
				tN_off=N_off=10;
				tN_on=N_on=8000;
				tim1.stop();
				tim1.compoutmodeA(1);
				tim1.compareA(compare);
				tim1.start(prescaler);
			}
			//Adjust Impulses off and impulses on
			//off decrement
			if(input=='*'){
				tN_on-=1;
				if(tN_on>N_on || tN_on<0)
					tN_on=N_on=0;
				else
					N_on=tN_on;
			}
			if(input=='0'){
				tN_on-=10;
				if(tN_on>N_on)
					tN_on=N_on=0;
				else
					N_on=tN_on;
			}
			//off increment
			if(input=='7'){
				tN_on+=1;
				if(tN_on<N_on)
					tN_on=N_on=65535;
				else
					N_on=tN_on;
			}
			if(input=='8'){
				tN_on+=10;
				if(tN_on<N_on)
					tN_on=N_on=65535;
				else
					N_on=tN_on;
			}
			//on decrement
			if(input==35){
				tN_off-=1;
				if(tN_off>N_off || tN_off<0)
					tN_off=N_off=0;
				else
					N_off=tN_off;
			}
			//on increment
			if(input=='9'){
				tN_off+=1;
				if(tN_off<N_off)
					tN_off=N_off=65535;
				else
					N_off=tN_off;
			}
			//Adjust compare OCR1A
			//decrement
			if(input=='4'){
				//tim1.stop();
				tcompare-=1;
				if(tcompare>compare || tcompare<3)
					tcompare=compare=3;
				else
					compare=tcompare;
				tim1.compareA(compare);
				//tim1.start(prescaler);
			}
			if(input=='5'){
				//tim1.stop();
				tcompare-=10;
				if(tcompare>compare)
					tcompare=compare=3;
				else
					compare=tcompare;
				tim1.compareA(compare);
				//tim1.start(prescaler);
			}
			if(input=='6'){
				//tim1.stop();
				tcompare-=100;
				if(tcompare>compare)
					tcompare=compare=3;
				else
					compare=tcompare;
				tim1.compareA(compare);
				//tim1.start(prescaler);
			}
			if(input=='B'){
				//tim1.stop();
				tcompare-=1000;
				if(tcompare>compare)
					tcompare=compare=3;
				else
					compare=tcompare;
				tim1.compareA(compare);
				//tim1.start(prescaler);
			}
			//increment
			if(input=='1'){
				//tim1.stop();
				tcompare+=1;
				if(tcompare<compare)
					tcompare=compare=65535;
				else
					compare=tcompare;
				tim1.compareA(compare);
				//tim1.start(prescaler);
			}
			if(input=='2'){
				//tim1.stop();
					tcompare+=10;
				if(tcompare<compare)
					tcompare=compare=65535;
				else
					compare=tcompare;
				tim1.compareA(compare);
				//tim1.start(prescaler);
			}
			if(input=='3'){
				//tim1.stop();
					tcompare+=100;
				if(tcompare<compare)
					tcompare=compare=65535;
				else
					compare=tcompare;
				tim1.compareA(compare);
				//tim1.start(prescaler);
			}
			if(input=='A'){
				//tim1.stop();
					tcompare+=1000;
				if(tcompare<compare)
					tcompare=compare=65535;
				else
				compare=tcompare;
				tim1.compareA(compare);
				//tim1.start(prescaler);
			}
			//Adjust prescaler 'C' toggles between possible selections
			if(input=='C'){
				switch(steprescaler){
					case 0:
						prescaler=1;
						steprescaler=1;
						break;
					case 1:
						prescaler=8;
						steprescaler=2;
						break;
					case 2:
						prescaler=64;
						steprescaler=3;
						break;
					case 3:
						prescaler=256;
						steprescaler=4;
						break;
					case 4:
						prescaler=1024;
						steprescaler=0;
						break;
					default:
						steprescaler=4;
						break;		
				}//endswitch
				tim1.stop();
				tim1.start(prescaler);
			}//endif
			//Parameters save eeprom
			eeprom.update_word((uint16_t*)0,compare);
			eeprom.update_word((uint16_t*)4,prescaler);
			eeprom.update_word((uint16_t*)8,N_off);
			eeprom.update_word((uint16_t*)12,N_on);
		}//endif
		/***DISPLAY***/
		//Parameters
		func.i32toa(compare,value);
		lcd0.gotoxy(2,0);
		lcd0.string_size(value,5);
		lcd0.BF();
		lcd0.hspace(1);
		lcd0.string("at");
		lcd0.hspace(1);
		func.i32toa(prescaler,value);
		lcd0.string_size(value,5);
		lcd0.BF();
		//Cycle
		func.i32toa(N_on/2,value);
		lcd0.gotoxy(3,0);
		lcd0.string_size(value,5);
		lcd0.BF();
		lcd0.hspace(1);
		lcd0.string("and");
		lcd0.hspace(1);
		func.i32toa(N_off/2,value);
		lcd0.string_size(value,5);
		lcd0.BF();
		//Default
		
    }//endwhile
}//endmain
/***Prototypes***/
void PORTINIT(void){
	DDRD = (1<<0) | (1<<4) | (1<<5);
	PORTD = (1<<4) | (1<<5);
};
/***Interrupts***/
ISR(TIMER1_COMPA_vect)
{
	counter++;
	if(counter>N_off)
		PORTD &= ~(1<<4);
	if(counter>(N_off+N_on)){
		PORTD |=(1<<4);
		counter=0;
	}
}
/***EOF***/
