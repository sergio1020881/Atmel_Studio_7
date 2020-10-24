/*************************************************************************
Title: 74HC595 API
Author: Sergio Santos 
	<sergio.salazar.santos@gmail.com>
File: $Id: 74hc595.c Exp $
License: GNU General Public License        
Comment:
	Tested Atemga88 8Mhz and Atmega328 8Mhz
	reviewed 09/10/2020                    
************************************************************************/
#ifndef F_CPU
/***Mandatory to use util/delay.h***/
	#define F_CPU 16000000UL
#endif
/*
** Library
*/
#include <avr/io.h>
#include <inttypes.h>
/***/
#include "74hc595.h"
/*
** constant and macro
*/
#define GLOBAL_INTERRUPT_ENABLE 7
#define ZERO 0
/*
** variable
*/
volatile uint8_t *hc595_DDR;
volatile uint8_t *hc595_PORT;
uint8_t HC595_datapin; 
uint8_t HC595_clkpin; 
uint8_t HC595_outpin;
/*
** procedure and function header
*/
void HC595_shift_bit(uint8_t bool);
void HC595_shift_byte(uint8_t byte);
void HC595_shift_out(void);
/*
** procedure and function
*/
HC595 HC595enable(volatile uint8_t *ddr, volatile uint8_t *port, uint8_t datapin, uint8_t clkpin, uint8_t outpin)
{
	//LOCAL VARIABLES
	uint8_t tSREG;
	tSREG=SREG;
	SREG&=~(1<<GLOBAL_INTERRUPT_ENABLE);
	//ALLOCAÇÂO MEMORIA PARA Estrutura
	HC595 hc595;
	//import parametros
	hc595_DDR=ddr;
	hc595_PORT=port;
	HC595_datapin=datapin; 
	HC595_clkpin=clkpin; 
	HC595_outpin=outpin;
	//inic variables
    *hc595_DDR |= (1<<datapin) | (1<<clkpin) | (1<<outpin);
	*hc595_PORT &= ~((1<<datapin) | (1<<clkpin) | (1<<outpin));
	//Direccionar apontadores para PROTOTIPOS
	hc595.bit=HC595_shift_bit;
	hc595.byte=HC595_shift_byte;
	hc595.out=HC595_shift_out;
	SREG=tSREG;
	//
	return hc595;
}
void HC595_shift_bit(uint8_t bool)
{
	if (bool)
		*hc595_PORT |= (1<<HC595_datapin); //Data bit HIGH
	else
		*hc595_PORT &= ~(1<<HC595_datapin); //Data bit LOW
	*hc595_PORT |= (1<<HC595_clkpin); // Shift bit
	*hc595_PORT &= ~(1<<HC595_clkpin); //Shift disable
}
void HC595_shift_byte(uint8_t byte)
{
	uint8_t i;
	for(i=0;i<8;i++)
		HC595_shift_bit(byte & (1<<i));
	HC595_shift_out();
}
void HC595_shift_out(void)
{
	*hc595_PORT |= (1<<HC595_outpin); //Output enable
	*hc595_PORT &= ~(1<<HC595_outpin); //Output disable
}
/*
** interrupt
*/
/***EOF***/

