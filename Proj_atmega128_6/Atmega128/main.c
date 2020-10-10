/*
	Atmega128.c
Created: 09/10/2020 20:25:13
Author: Sérgio Santos
	<sergio.salazar.santos@gmail.com>
Comment:
	Create RTC lib
 */
/***Preamble***/
#define F_CPU 16000000UL
/***Library***/
#include <avr/io.h>
#include <util/delay.h>
#include "atmega128twi.h"
#include "lcd.h"

/******/
#define RTCC_R      0xa3
#define RTCC_W      0xa2
int main(void)
{
	uint8_t Rtcc_Addr = RTCC_R>>1;
	I2C twi = I2Cenable(Rtcc_Addr,0);
	LCD0 lcd = LCD0enable(&DDRA,&PINA,&PORTA);
    /* Replace with your application code */
    while (1) 
    {
		lcd.gotoxy(0,0);
		lcd.string_size("start",5);
		twi.master_connect(Rtcc_Addr,1);
		lcd.gotoxy(1,0);
		lcd.putch(twi.master_read(1));
    }
}

