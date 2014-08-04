/*
 * adc.h
 *
 *  Created on: 12-10-2013
 *      Author: pholat
 */

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void adc_on(uint8_t multiplexer,uint8_t status);
uint16_t adc_one(uint8_t pin);
uint16_t * adc_multiple(uint8_t pins)	;	// gimme like that /*ADC0,1,2 pin*/ ADC0|ADC1|ADC2
void adc_off(void);


#endif /* ADC_H_ */
