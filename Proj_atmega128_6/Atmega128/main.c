/*
	Atmega128.c
Created: 09/10/2020 20:25:13
Author: Sérgio Santos
	<sergio.salazar.santos@gmail.com>
Comment:
	Create PCF8563RTC lib
 */
/***Preamble***/
#define F_CPU 16000000UL
/***Library***/
#include <avr/io.h>
#include <util/delay.h>
//#include "atmega128twi.h"
//#include "Rtc_Pcf8563.h"
#include "lcd.h"
#include "i2c.h"
#include "pcf8563rtc.h"
#include "function.h"
/***Define and Macros***/
/***MAIN_MAIN***/
int main(void)
{
	uint8_t sec;
	LCD0 lcd = LCD0enable(&DDRA,&PINA,&PORTA);
	FUNC func = FUNCenable();
    PCF8563RTC_Init();
    //PCF8563RTC_SetTime(0x10,0x40,0x00);  //  10:40:20 am
    //PCF8563RTC_SetDate(0x01,0x01,0x15);  //  1st Jan 2015
	/* Replace with your application code */
	while (1)
    {
		
		lcd.gotoxy(0,0);
		lcd.string_size("start",5);
		
		/* Read the Time from RTC(PCF8563) */ 
		sec=PCF8563RTC_GetTime().VL_seconds;
		
		/* Read the Date from RTC(PCF8563) */
		PCF8563RTC_GetDate();
		
		lcd.gotoxy(2,0);
		lcd.string_size(func.ui16toa(PCF8563RTC_bcd2dec(sec)),2);
    }
}
/*EOF*/

