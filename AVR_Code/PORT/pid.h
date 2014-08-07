/*
 * pid.h
 *
 *  Created on: 15-05-2014
 *      Author: pholat
 *
 *      Easiest PID control.
 */

#ifndef PID_H_
#define PID_H_

#include <avr/io.h>
#include "main.h"
#include "periphs.h"
#include "pt1000.h"

volatile uint8_t PID_Flag=0;

uint16_t KP=100;
uint16_t KD=0;
uint16_t KI=0;

/* sum_error - sum error needed for KP
 * sub_error - subtraction error needed for  KD
 * set - calculations
 * old_error - old error for next calculations
 */

uint8_t pid(int16_t error)
{
	static uint16_t old_error=0,sum_error=0,sub_error=0;
	sum_error=sum_error+error;
	sub_error=error-old_error;
	static uint8_t set = 0;
	set=KP*error + KI*sum_error + KD*sub_error;
	old_error=error;
	// Set value between 0-PWM_MAX
	(set>PWM_MAX) ? (set=PWM_MAX) : (set < 1 ? set=0 : set);
	return set;
}

uint16_t pid_calc()
{
	uint16_t error = pgm_read_word_near(PT1000 + Get_Measurement());
	error = error - Get_Reference();
	return error;
}

#endif /* PID_H_ */
