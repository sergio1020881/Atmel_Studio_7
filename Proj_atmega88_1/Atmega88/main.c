/*************************************************
	led_dimmer.c
Created: 07/10/2020 19:31:17
Author : Sergio Santos
	<sergio.salazar.santos@gmail.com>
Comment:
	stable
**************************************************/
/***Pre Processor***/
#ifndef F_CPU
	#define F_CPU 8000000UL
#endif
/***Library***/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
/***Define and Macro***/
#define TRUE 1
#define REPEAT 100 // 100
/***Global variable***/
static volatile uint16_t adc_tmp;
int count;
/***Prototype***/
void PORTINIT(void);
void TIMER0ASETUP(void);
void TIMER1A_SETUP(uint8_t wagenmode, uint8_t compoutAmode,uint8_t interruptmask);
void TIMER1A_STARTSTOP(uint16_t prescaler);
void TIMER1A_trigger(uint16_t Atrigger);
void TIMER1_intcapture(uint16_t capture);
void ADC_ENABLE(uint8_t ADC_channel, uint8_t ADC_Vref, uint8_t ADC_lar, uint8_t ADC_Div);
/***MAIN_MAIN_MAIN***/
int main(void)
{
	//PORTINIT();
	/******/
	TIMER0ASETUP();
	count=REPEAT;
	/******/
	TIMER1A_SETUP(5,3,0);
	TIMER1A_trigger(128);
	TIMER1A_STARTSTOP(8);
	/******/
	ADC_ENABLE(0, 1, 0, 128);
    while(TRUE)
    {
    }
}
/***Procedure and Function***/
void PORTINIT(void)
{
}
/*****/
void TIMER0ASETUP(void){
	DDRD=(1<<PORTD6);
	/***T=10ms***/
	TCCR0A=(1<<WGM01); //|(1<<WGM00); CTC
	TIMSK0=(1<<OCIE0A); // OCR0A interrupção ativada;
	OCR0A=156; // 10ms implica OCR0A=156
	TCCR0B|=(1<<CS02); //|(1<<CS01); // N=256 e inicia contador
}
/*****/
void TIMER1A_SETUP(uint8_t wavegenmode, uint8_t compoutAmode,uint8_t interruptmask)
{
	uint8_t TCCR1A_tmp=0x00;
	uint8_t TCCR1B_tmp=0x00;
	uint8_t TIMSK1_tmp=0x00;
	switch(wavegenmode){
		case 0:
			break;
		case 1:
			TCCR1A_tmp=(1<<WGM10);
			break;
		case 2:
			TCCR1A_tmp=(1<<WGM11);
			break;
		case 3:
			TCCR1A_tmp=(3<<WGM10);
			break;
		case 4:
			TCCR1B_tmp=(1<<WGM12);
			break;
		case 5:
			TCCR1A_tmp=(1<<WGM10);
			TCCR1B_tmp=(1<<WGM12);
			break;
		case 6:
			TCCR1A_tmp=(1<<WGM11);
			TCCR1B_tmp=(1<<WGM12);
			break;
		case 7:
			TCCR1A_tmp=(3<<WGM10);
			TCCR1B_tmp=(1<<WGM12);
			break;
		case 8:
			TCCR1B_tmp=(1<<WGM13);
			break;
		case 9:
			TCCR1A_tmp=(1<<WGM10);
			TCCR1B_tmp=(1<<WGM13);
			break;
		case 10:
			TCCR1A_tmp=(1<<WGM11);
			TCCR1B_tmp=(1<<WGM13);
			break;
		case 11:
			TCCR1A_tmp=(3<<WGM10);
			TCCR1B_tmp=(1<<WGM13);
			break;
		case 12:
			TCCR1B_tmp=(3<<WGM12);
			break;
		case 14:
			TCCR1A_tmp=(1<<WGM11);
			TCCR1B_tmp=(3<<WGM12);
			break;
		case 15:
			TCCR1A_tmp=(3<<WGM10);
			TCCR1B_tmp=(3<<WGM12);
			break;
		default:
			break;
	};
	switch(compoutAmode){
		case 0:
			break;
		case 1:
			TCCR1A_tmp|=(1<<COM1A0);
			break;
		case 2:
			TCCR1A_tmp|=(1<<COM1A1);
			break;
		case 3:
			TCCR1A_tmp|=(3<<COM1A0);
			break;
		default:
			break;
	};
	switch(interruptmask){
		case 0:
			break;
		case 1:
			TIMSK1_tmp=(1<<TOIE1);
			break;
		case 2:
			TIMSK1_tmp=(1<<OCIE1A);
			break;
		case 3:
			TIMSK1_tmp=(3<<TOIE1);
			break;
		case 4:
			TIMSK1_tmp=(1<<OCIE1B);
			break;
		case 5:
			TIMSK1_tmp=((1<<OCIE1B)|(1<<TOIE1));
			break;
		case 6:
			TIMSK1_tmp=((3<<OCIE1A));
			break;
		case 7:
			TIMSK1_tmp=(7<<TOIE1);
			break;
		case 8:
			TIMSK1_tmp=(1<<ICIE1);
			break;
		case 9:
			TIMSK1_tmp=((1<<ICIE1)|(1<<TOIE1));
			break;
		case 10:
			TIMSK1_tmp=((1<<ICIE1)|(1<<OCIE1A));
			break;
		case 11:
			TIMSK1_tmp=((1<<ICIE1)|(3<<TOIE1));
			break;
		case 12:
			TIMSK1_tmp=((1<<ICIE1)|(1<<OCIE1B));
			break;
		case 13:
			TIMSK1_tmp=((1<<ICIE1)|(1<<OCIE1B)|(1<<TOIE1));
			break;
		case 14:
			TIMSK1_tmp=((1<<ICIE1)|(3<<OCIE1A));
			break;
		case 15:
			TIMSK1_tmp=((1<<ICIE1)|(7<<TOIE1));
			break;
		default:
			break;
	};
	TCCR1A=TCCR1A_tmp;
	TCCR1B=TCCR1B_tmp;
	if(compoutAmode){
		DDRB|=(1<<PB1);
	}
	TIMSK1=TIMSK1_tmp;
}
void TIMER1A_STARTSTOP(uint16_t prescaler)
{
	uint8_t TCCR1B_tmp=0x00;
	switch(prescaler){
		case 0:
			TCCR1B&=~((1<<CS12)|(1<<CS11)|(1<<CS10));
			break;
		case 1:
			TCCR1B_tmp=(1<<CS10);
			break;
		case 8:
			TCCR1B_tmp=(1<<CS11);
			break;
		case 64:
			TCCR1B_tmp=(3<<CS10);
			break;
		case 256:
			TCCR1B_tmp=(1<<CS12);
			break;
		case 1024:
			TCCR1B_tmp=((1<<CS12)|(1<<CS10));
			break;
		case 2:
			TCCR1B_tmp=(3<<CS11);
			break;
		case 3:
			TCCR1B_tmp=((1<<CS12)|(1<<CS10));
			break;
		default:
			TCCR1B_tmp=(7<<CS10);
	}
	TCCR1B|=TCCR1B_tmp;
}
void TIMER1A_trigger(uint16_t Atrigger)
{
	OCR1A=Atrigger;
}
void TIMER1_intcapture(uint16_t capture)
{
	ICR1=capture;
}
/*****/
void ADC_ENABLE(uint8_t ADC_channel, uint8_t ADC_Vref, uint8_t ADC_lar, uint8_t ADC_Div)
{
	uint8_t ADMUX_tmp;
	uint8_t ADCSRA_tmp;
	switch(ADC_channel){
		case 0:
			DDRC&=~(1<<PC0);
			ADMUX_tmp=0x00;
			break;
		case 1:
			DDRC&=~(1<<PC1);
			ADMUX_tmp=0x01;
			break;
		case 2:
			DDRC&=~(1<<PC2);
			ADMUX_tmp=0x02;
			break;
		case 3:
			DDRC&=~(1<<PC3);
			ADMUX_tmp=0x03;
			break;
		case 4:
			DDRC&=~(1<<PC4);
			ADMUX_tmp=0x04;
			break;
		case 5:
			DDRC&=~(1<<PC5);
			ADMUX_tmp=0x05;
			break;
		case 6:
			ADMUX_tmp=0x06;
			break;
		case 7:
			ADMUX_tmp=0x07;
			break;
		default:
		ADMUX_tmp=0x00;
	};
	switch(ADC_Vref){
		case 0:
			break;
		case 1:
			ADMUX_tmp|=(1<<REFS0);
			break;
		case 3:
			ADMUX_tmp|=(3<<REFS0);
			break;
		default:
			ADMUX_tmp|=(1<<REFS0);
	};
	switch(ADC_lar){
		case 0:
			break;
		case 1:
			ADMUX_tmp|=(1<<ADLAR);
			break;
		default:
			break;
	};
	switch(ADC_Div){
		case 1:
			ADCSRA_tmp=(1<<ADPS0);
			break;
		case 2:
			ADCSRA_tmp=(1<<ADPS0);
			break;
		case 4:
			ADCSRA_tmp=(1<<ADPS1);
			break;
		case 8:
			ADCSRA_tmp=(3<<ADPS0);
			break;
		case 16:
			ADCSRA_tmp=(1<<ADPS2);
			break;
		case 32:
			ADCSRA_tmp=(5<<ADPS0);
			break;
		case 64:
			ADCSRA_tmp=(3<<ADPS1);
			break;
		case 128:
			ADCSRA_tmp=(7<<ADPS0);
			break;
		default:
			ADCSRA_tmp=(7<<ADPS0);
			break;
	};
	ADMUX=ADMUX_tmp;
	ADCSRA_tmp|=(1<<ADIE);
	ADCSRA_tmp|=(1<<ADEN);
	ADCSRA_tmp|=(1<<ADSC);
	ADCSRA_tmp|=(1<<ADATE);
	ADCSRA=ADCSRA_tmp;
	ADCSRB&=~(7<<ADTS0);
	sei();
}	
/***Interrupt***/
ISR(TIMER0_COMPA_vect){
	uint8_t sREG=SREG;
	cli();
	count--;
	if(!count){
		PORTD^=(1<<PORTD6);
		count=REPEAT;
	}
	SREG=sREG;
	sei();
}
/******/
ISR(ADC_vect)
{
	//uint8_t sREG=SREG;
	//cli();
	adc_tmp=ADCL;
	adc_tmp|=(ADCH<<8);
	TIMER1A_trigger(adc_tmp>>2);
	//SREG=sREG;
	//sei();
}
/***EOF***/


