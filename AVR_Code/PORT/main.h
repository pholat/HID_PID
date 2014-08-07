/*
 * main.h
 *
 *  Created on: 23-05-2014
 *      Author: pholat
 */

#ifndef MAIN_H_
#define MAIN_H_

#define PWM_MAX 1000

volatile static uint16_t ms=0;
volatile static uint8_t flag=0;		// == minutes
volatile static uint16_t decys=0;		// == decy seconds
volatile static uint16_t pwm=0;

void ms_delay(uint16_t what);
int16_t Get_Reference(void);

#endif /* MAIN_H_ */
