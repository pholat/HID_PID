/*
 * n5110.h
 *
 *  Created on: 06-10-2013
 *      Author: pholat
 */

#include <avr/io.h>
#include <util/delay.h>
#include "ascii.h"

#ifndef N5110_H_
#define N5110_H_

#ifndef setbit
#define setbit(x,y) 	x|=(1<<y)
#endif

#ifndef clearbit
#define clearbit(x,y) 	x&=~(1<<y)
#endif

#ifndef negbit
#define negbit(x,y) 	x^=(1<<y)
#endif

#ifndef checkbit
#define checkbit(x,y)	x&(1<<y)
#endif

// poprawione
#define SCE PD7//PB0				// MUST CHANGE colides with nRF -> need PD6
#define RES PD4				// actual
#define DC PD5				// actual
#define SDIN PB3			// standard MOSI
#define SCKL PB5			// standard CLCK
// a to używane
#define CMD	 	0										// comand send
#define IMG		1										// image send
#define LCD_X 84										// wielkosc w x
#define LCD_Y 48										// wielkosc w y

void lcd_write_byte(unsigned char screen,uint8_t data);
void lcd_init(void);
void lcd_dot(void);
void lcd_char(char character);
void lcd_string(char *stringi);
void lcd_place(uint8_t,uint8_t);
void lcd_clear(void);
void lcd_int(uint16_t);
void lcd_bin(uint8_t);



#endif /* N5110_H_ */
