/************************************************************************
	INTERRUPT
Author: Sergio Santos 
	<sergio.salazar.santos@gmail.com>
Date: 28092020
Hardware: ATmega128
Comment:
	Stable
************************************************************************/
/***Preamble Inic***/
#ifndef _ATMEGA128INTERRUPT_H_
	#define _ATMEGA128INTERRUPT_H_
/**@{*/
#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
	#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif
/***Library***/
#include <inttypes.h>
/***Constant & Macro***/
/***Global Variable***/
struct ntrrpt{
	void (*set)(uint8_t channel, uint8_t sense);
	void (*off)(uint8_t channel);
	uint8_t (*reset_status)(void);
};
typedef struct ntrrpt INTERRUPT;
/***Header***/
INTERRUPT INTERRUPTenable(void);
#endif
/***EOF***/
