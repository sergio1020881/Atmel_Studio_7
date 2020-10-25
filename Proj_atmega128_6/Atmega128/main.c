/*
	Atmega128.c
Created: 09/10/2020 20:25:13
Author: Sérgio Santos
	<sergio.salazar.santos@gmail.com>
Hardware: Atmega 128
	-PORTA LCD
	-PORTG HC595
	-PORTD PCF8563 (I2C)
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
#include "74hc595.h"
/***Define and Macros***/
#define TRUE 1
#define ZERO 0
/***Global File Variable***/
struct time tmp;
struct date dt;
/***MAIN_MAIN***/
int main(void)
{
	//uint8_t i;
	LCD0 lcd = LCD0enable(&DDRA,&PINA,&PORTA);
	FUNC func = FUNCenable();
	HC595 shift = HC595enable(&DDRG,&PORTG,2,0,1);
	_delay_ms(100);
    PCF8563RTC_Init(64);
    //PCF8563RTC_SetTime(0x23,0x50,0x00);  //  23:59:20 am
    //PCF8563RTC_SetDate(0x10,0x00,0x10,0x19);
	PCF8563RTC_SetClkOut(1, 2);
	/* Replace with your application code */
	while (1)
    {
		lcd.reboot();
		
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
		lcd.hspace(7);
		lcd.string_size(func.ui16toa(tmp.VL_seconds),2);
		
		dt=PCF8563RTC_GetDate();
		lcd.gotoxy(2,0);
		lcd.string_size(func.ui16toa(PCF8563RTC_bcd2dec(dt.days)),2);
		//lcd.putch(':');
		//lcd.string_size(func.ui16toa(PCF8563RTC_bcd2dec(dt.weekdays & ~0xF8)),2);
		lcd.putch(':');
		lcd.string_size(func.ui16toa(PCF8563RTC_bcd2dec(dt.century_months & ~0xE0)),2);
		lcd.putch(':');
		lcd.string_size(func.ui16toa(PCF8563RTC_bcd2dec(dt.years)),2);
		
			shift.bit(0);
			shift.out();
			_delay_ms(50);
			shift.bit(1);
			shift.out();
			_delay_ms(50);
		
    }
}
/*EOF*/

