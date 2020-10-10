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
//#include "atmega128twi.h"
//#include "Rtc_Pcf8563.h"
#include "lcd.h"
#include "i2c.h"
#include "rtc.h"
#include "function.h"
/***Define and Macros***/
/*rtc start PCF8563*/
#define RTCC_R      0xa3 //address
#define RTCC_W      0xa2 //address
#define RTCC_SEC        1
#define RTCC_MIN        2
#define RTCC_HR         3
#define RTCC_DAY        4
#define RTCC_WEEKDAY    5
#define RTCC_MONTH      6
#define RTCC_YEAR       7
#define RTCC_CENTURY    8
// register addresses in the rtc
#define RTCC_STAT1_ADDR     0x0
#define RTCC_STAT2_ADDR     0x01
#define RTCC_SEC_ADDR       0x02
#define RTCC_MIN_ADDR       0x03
#define RTCC_HR_ADDR        0x04
#define RTCC_DAY_ADDR       0x05
#define RTCC_WEEKDAY_ADDR   0x06
#define RTCC_MONTH_ADDR     0x07
#define RTCC_YEAR_ADDR      0x08
#define RTCC_ALRM_MIN_ADDR  0x09
#define RTCC_SQW_ADDR       0x0D
#define RTCC_TIMER1_ADDR    0x0E
#define RTCC_TIMER2_ADDR    0x0F
// setting the alarm flag to 0 enables the alarm.
// set it to 1 to disable the alarm for that value.
#define RTCC_ALARM          0x80
#define RTCC_ALARM_AIE      0x02
#define RTCC_ALARM_AF       0x08
// optional val for no alarm setting
#define RTCC_NO_ALARM       99
#define RTCC_TIMER_TIE      0x01  // Timer Interrupt Enable
#define RTCC_TIMER_TF       0x04  // Timer Flag, read/write active state
                                  // When clearing, be sure to set RTCC_TIMER_AF
                                  // to 1 (see note above).
#define RTCC_TIMER_TI_TP    0x10  // 0: INT is active when TF is active
                                  //    (subject to the status of TIE)
                                  // 1: INT pulses active
                                  //    (subject to the status of TIE);
                                  // Note: TF stays active until cleared
                                  // no matter what RTCC_TIMER_TI_TP is.
#define RTCC_TIMER_TD10     0x03  // Timer source clock, TMR_1MIN saves power
#define RTCC_TIMER_TE       0x80  // Timer 1:enable/0:disable
// Timer source-clock frequency constants
#define TMR_4096HZ      B00000000
#define TMR_64Hz        B00000001
#define TMR_1Hz         B00000010
#define TMR_1MIN        B00000011
#define RTCC_CENTURY_MASK   0x80
#define RTCC_VLSEC_MASK     0x80
// date format flags
#define RTCC_DATE_WORLD     0x01
#define RTCC_DATE_ASIA      0x02
#define RTCC_DATE_US        0x04
// time format flags
#define RTCC_TIME_HMS       0x01
#define RTCC_TIME_HM        0x02
// square wave contants
#define SQW_DISABLE     B00000000
#define SQW_32KHZ       B10000000
#define SQW_1024HZ      B10000001
#define SQW_32HZ        B10000010
#define SQW_1HZ         B10000011
/*rtc end PCF8563*/
/***MAIN_MAIN***/
int main(void)
{
	//uint8_t Rtcc_Addr = RTCC_R>>1;
	uint8_t hour,min,sec,day,month,year = 0;
	//I2C twi = I2Cenable(Rtcc_Addr,4);
	LCD0 lcd = LCD0enable(&DDRA,&PINA,&PORTA);
	FUNC func = FUNCenable();
    /* Replace with your application code */
	/***rtc init start***/
	//I2C_Init(Rtcc_Addr,4);
	//I2C_Start();                            // Start I2C communication
	//I2C_Write(Rtcc_Addr);                   // Connect to DS1307 by sending its ID on I2c Bus
	//I2C_Write(0x03);                           // Select the Ds1307 ControlRegister to configure Ds1307
	//I2C_Write(0x00);                        // Write 0x00 to Control register to disable SQW-Out
	//I2C_Stop();
	/***rtc init end***/
    RTC_Init();
	/*$$$$ Set the time and Date only once. Once the Time and Date is comment these lines
    and reflash the code. Else the time will be reset every time the controller is reset*/
    RTC_SetTime(0x10,0x40,0x00);  //  10:40:20 am
    RTC_SetDate(0x01,0x01,0x15);  //  1st Jan 2015
	while (1)
    {
		
		lcd.gotoxy(0,0);
		lcd.string_size("start",5);
		
		/* Read the Time from RTC(ds1307) */ 
		RTC_GetTime(&hour,&min,&sec);
		
		/* Read the Date from RTC(ds1307) */
		RTC_GetDate(&day,&month,&year);
		
		
		
		
		
		
		lcd.gotoxy(1,0);
		lcd.string(func.i16toa(func.bcd2dec(sec)));
    }
}

