/*
 * spi.h
 *
 *  Created on: 14-10-2013
 *      Author: pholat
 */

#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
void spi_init(void);
uint8_t spi_write_byte(uint8_t cData);


#endif /* SPI_H_ */
