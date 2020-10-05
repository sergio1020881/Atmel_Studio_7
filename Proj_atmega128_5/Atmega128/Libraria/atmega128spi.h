/************************************************************************
ATMEGA128 SPI API START
Author: Sergio Santos 
	<sergio.salazar.santos@gmail.com>
	28092020
************************************************************************/
/***preamble inic***/
#ifndef _ATMEGA128SPI_H_
	#define _ATMEGA128SPI_H_
/**@{*/
#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
	#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif
/*
** Library
*/
#include <inttypes.h>
/*************************************************************************
SPI API START
*************************************************************************/
/*
** constant and macro
*/
#define SPI_LSB_DATA_ORDER 1
#define SPI_MSB_DATA_ORDER 0
#define SPI_MASTER_MODE 1
#define SPI_SLAVE_MODE 0
/*
** variable
*/
struct sp{
	/***/
	void (*transfer_sync) (uint8_t * dataout, uint8_t * datain, uint8_t len);
	void (*transmit_sync) (uint8_t * dataout, uint8_t len);
	uint8_t (*fast_shift) (uint8_t data);
};
typedef struct sp SPI;
/*
** procedure and function header
*/
SPI SPIenable(uint8_t master_slave_select, uint8_t data_order,  uint8_t data_modes, uint8_t prescaler);
/*************************************************************************
SPI API END
*************************************************************************/
#endif
/***EOF***/
