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
/***Global Variable***/
struct time tmp;
struct date dt;
/***MAIN_MAIN***/
int main(void)
{
	LCD0 lcd = LCD0enable(&DDRA,&PINA,&PORTA);
	FUNC func = FUNCenable();
    PCF8563RTC_Init(64);
    PCF8563RTC_SetTime(0x10,0x59,0x00);  //  10:40:20 am
    PCF8563RTC_SetDate(0x01,0x01,0x15);  //  1st Jan 2015
	/* Replace with your application code */
	while (1)
    {
		
		lcd.gotoxy(0,0);
		lcd.string_size("start",5);
		
		/* Read the Time from RTC(PCF8563) */ 
		tmp=PCF8563RTC_GetTime();
		
		lcd.gotoxy(1,0);
		lcd.string_size(func.ui16toa(PCF8563RTC_bcd2dec(tmp.hours)),2);
		lcd.putch(':');
		lcd.string_size(func.ui16toa(PCF8563RTC_bcd2dec(tmp.minutes)),2);
		lcd.putch(':');
		lcd.string_size(func.ui16toa(PCF8563RTC_bcd2dec(tmp.VL_seconds)),2);
		
		dt=PCF8563RTC_GetDate();
		lcd.gotoxy(2,0);
		lcd.string_size(func.ui16toa(PCF8563RTC_bcd2dec(dt.days)),2);
		lcd.putch(':');
		lcd.string_size(func.ui16toa(PCF8563RTC_bcd2dec(dt.weekdays)),2);
		lcd.putch(':');
		lcd.string_size(func.ui16toa(PCF8563RTC_bcd2dec(dt.century_months)),2);
		lcd.putch(':');
		lcd.string_size(func.ui16toa(PCF8563RTC_bcd2dec(dt.years)),2);
    }
}
/*EOF*/

