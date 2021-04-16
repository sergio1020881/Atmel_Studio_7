/********************************************************************
	EXPLODE
Author: Sergio Santos
	<sergio.salazar.santos@gmail.com> 
License: GNU General Public License
Hardware: all
Date: 16032021
Comment:
    Pin Analysis
********************************************************************/
/***Library***/
#include <avr/io.h>
#include <inttypes.h>
#include"explode.h"
/***Constant & Macro***/
#ifndef GLOBAL_INTERRUPT_ENABLE
	#define STATUS_REGISTER SREG
	#define GLOBAL_INTERRUPT_ENABLE 7
#endif
#define ZERO 0
#define ONE 1
/***Global File Variable***/
/***Header***/
uint8_t EXPLODEPwr(uint8_t bs, uint8_t n);
/************/
void EXPLODEboot(EXPLODE* self, uint8_t x);
uint8_t EXPLODEmayia(EXPLODE* self, uint8_t nbits);
uint8_t EXPLODEhh(EXPLODE* self);
uint8_t EXPLODEll(EXPLODE* self);
uint8_t EXPLODElh(EXPLODE* self);
uint8_t EXPLODEhl(EXPLODE* self);
uint8_t EXPLODEdiff(EXPLODE* self);
uint8_t EXPLODEdata(EXPLODE* self);
/***Procedure & Function***/
EXPLODE EXPLODEenable( void )
{
	uint8_t tSREG;
	tSREG = STATUS_REGISTER;
	STATUS_REGISTER &= ~(ONE<<GLOBAL_INTERRUPT_ENABLE);
	// struct object
	struct expld explode;
	// inic VAR
	explode.XI = ZERO;
	explode.XF = ZERO;
	// function pointers
	explode.boot = EXPLODEboot;
	explode.mayia = EXPLODEmayia;
	explode.hh = EXPLODEhh;
	explode.ll = EXPLODEll;
	explode.lh = EXPLODElh;
	explode.hl = EXPLODEhl;
	explode.diff = EXPLODEdiff;
	explode.data = EXPLODEdata;
	STATUS_REGISTER = tSREG;
	/******/
	return explode;
}
// boot
void EXPLODEboot(EXPLODE* self, uint8_t x)
{
	self->XI = self->XF;
	self->XF = x;
}
// mayia
uint8_t EXPLODEmayia(EXPLODE* self, uint8_t nbits)
{//magic formula
	unsigned int mask;
	unsigned int diff;
	unsigned int trans;
	mask = EXPLODEPwr(2,nbits)-ONE;
	self->XI &= mask;
	self->XF &= mask;
	diff = self->XF ^ self->XI;
	trans = diff & self->XF;
	return (trans << nbits) | diff;
}
// hh
uint8_t EXPLODEhh(EXPLODE* self)
{
	uint8_t i;
	i = self->XI & self->XF;
	return i;
}
// ll
uint8_t EXPLODEll(EXPLODE* self)
{
	uint8_t i;
	i = self->XI | self->XF;
	return ~i;
}
// lh
uint8_t EXPLODElh(EXPLODE* self)
{
	uint8_t i;
	i = self->XI ^ self->XF;
	i &= self->XF;
	return i;
}
// hl
uint8_t EXPLODEhl(EXPLODE* self)
{
	uint8_t i;
	i = self->XF ^ self->XI;
	i &= self->XI;
	return i;
}
// diff
uint8_t EXPLODEdiff(EXPLODE* self)
{
	return self->XF ^ self->XI;
}
uint8_t EXPLODEdata(EXPLODE* self)
{
	return self->XF;	
}
/*******************************************************************/
// power: raise base to n-th power; n >= 0
uint8_t EXPLODEPwr(uint8_t bs, uint8_t n)
{
	uint8_t i, p;
	p = ONE;
	for (i = ONE; i <= n; ++i)
	p = p * bs;
	return p;
}
/***Interrupt***/
/***Comment***
*************/
/***EOF***/
