/*
 * periphs.h
 *
 *  Created on: 22-05-2014
 *      Author: pholat
 */

#ifndef PERIPHS_H_
#define PERIPHS_H_

#include <avr/io.h>

void Init(void);
int16_t Get_Measurement(void);
void adc_on( uint8_t input );
void pwm_on();
/* void clock_on(); */

#endif /* PERIPHS_H_ */
