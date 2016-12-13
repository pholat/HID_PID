/*
 * main.c
 *
 *  Created on: 		15-05-2014
 *  Last update:		12-06-2014
 *  Comments updated: 	12-06-2014
 *  Author: pholat
 *
 *	Main contains control of:
 *	→ Peripherals initialization
 *	→ Delay function - which is used in whole project → def in main.h
 *	→ Get_Reference used in whole project too → def in main.h
 *
 *	How it works:
 *	→ Initialization
 *	→ Loop:
 *		→ PID gets computed and executed
 *		→ Error gets printed on screen
 *
 *	CAUTION: pt1000.h need exact values from matlab script.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "pid.h"
//#include "pt1000.h"			// Included in pid.h
#include "periphs.h"
#include "main.h"

#include "ascii.h"
#include "spi.h"
#include "n5110.h"

// TODO - PID regulation, PID values in pid.h
// TODO ADC in free running mode

//[*C/s *10] *C are scaled 100*, so gain is *10 & time *10 = 100
uint8_t przyrost1=20;
uint8_t przyrost2=28;

/* Initialization of spi, screen, timer2 and ADC */
void periph_init();
/* Function for accurate ms delays */
void ms_delay(uint16_t);
/* function providing reference temperature */
int16_t Get_Reference(void);

int main()
{
    /* variables for PID */
    uint16_t error;
    sei();
    while(1) {
        error=pid_calc();
        if(PID_Flag) {
            OCR2=pid(error);
            PID_Flag=0;
        }
        lcd_place(0,4);
        lcd_string("Err:");
        lcd_int(error/100);
    }
}

int16_t Get_Reference(void)
{
    uint16_t nastawa_temp=0;
    switch (flag) {
    case 0:
        // WARN: decys !> than 600 than zero!
        nastawa_temp=(przyrost1*decys);
        break;
    case 1:
        nastawa_temp=9000;
        break;
    case 6:
        // WARN: decys !> than 600 than zero!
        nastawa_temp=(przyrost2*decys)+9000;
        break;
    case 7:
        nastawa_temp=24500;
        break;
    case 9:
        nastawa_temp=2500;
        break;
    }
    return nastawa_temp;
}

/* Clock is set up by ums*/
ISR(TIMER2_COMP_vect)
{
    // ms = 0 by each ms
    (ms>999) ? (ms=0):(ms++);
    // decys ++ & pid() in each 0.1s
    (ms/100) ? decys++,PID_Flag=1:decys;
    // change flag is set and decys=0 each 1min.
    (decys>600) ? flag++ , decys=0: flag;

    // Program PWM
    // PWM = 1000 == 100%
    static uint16_t counter=0;
    (counter<1000) ? (counter++) : (counter=0);
    (pwm<counter) ? (PORTC&=~(1<<PC5)) : (PORTC|=(1<<PC5));
}

void ms_delay(uint16_t what)
{
    uint16_t is=ms+what;
    while(is!=ms);
}

void periph_init()
{
    adc_on(1);
    pwm_on();
    Init();
    spi_init();
    lcd_init();
    lcd_clear();
}
