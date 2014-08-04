/*
 * adc.c
 *
 *  Created on: 12-10-2013
 *      Author: pholat
 */


#include "adc.h"

void adc_on(uint8_t multiplexer,uint8_t status)
{
	ADMUX|=multiplexer;
	uint8_t a=1; uint8_t i=0;
	uint16_t clock=F_CPU/1000;
	while(clock/a>200)
		{
			a=(a<<1);
			i++;
		}
	if(i>7) i=7;
	ADCSRA|=i;
	ADCSRA|=status;
}

uint16_t adc_one(uint8_t pin)
{
	ADMUX&=~((1<<MUX0)|(1<<MUX1)|(1<<MUX2));
	ADMUX|=pin;
	ADCSRA|=(1<<ADSC);
	while(ADCSRA&(1<<ADSC));
	if(ADMUX&(1<<ADLAR)) return ADCH;
	else return ADC;

}

uint16_t * adc_multiple(uint8_t pins)
{
	static uint16_t walues[7];
	for(uint8_t i=0;i<6;i++) if(pins&(1<<i)) walues[i]=adc_one(i);
	return walues;
}

void adc_off(void)
{
	ADCSRA&=~(1<<ADEN);
}
