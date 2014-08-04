/*
 * spi.c
 *
 *  Created on: 14-10-2013
 *      Author: pholat
 */

#include "spi.h"

void spi_init(void)
{
	// outputs enable
	// 	   SCK 	   | MOSI 	 | CSN 	   | CE
	DDRB|=(1<<PB5)|(1<<PB3)|(1<<PB1)|(1<<PB0);
	// SS as output
	DDRB|=(1<<PB2);
	//SPI enable,master, prescaler 16
	SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPR0);

	/*	Stuff needed only for nRF!
	// (CSN) Clock Select Not → HI
	CSN_hi;
	// CE to LOW - in RX lo is standby, hi is receiving packets
	CE_hi;
	*/
}
				/* Writting byte by SPI */
uint8_t spi_write_byte(uint8_t cData)
{
	// load data to send
	SPDR = cData;
	// wait till end of transmission
	while(!(SPSR&(1<<SPIF)));
	// return data loaded during transmission
	return SPDR;
}
