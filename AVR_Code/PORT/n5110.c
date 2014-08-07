/*
 * n5110.c
 *
 *  Created on: 06-10-2013
 *      Author: pholat
 */

#include "n5110.h"

void lcd_write_byte(unsigned char screen,uint8_t data)
{
	PORTD&=~(1<<SCE);
	if(screen) PORTD|=(1<<DC);					// if IMG DC HI
	else PORTD&=~(1<<DC);						// else CMD DC LO
	SPDR = data;
	while(!(SPSR&(1<<SPIF)));
	PORTD|=(1<<SCE);
}

void lcd_init(void)
{
	PORTD&=~(1<<RES);			// low on RES

	DDRD|=(1<<RES)|(1<<DC)|(1<<SCE);					// out: RES,DC
	DDRB|=(1<<SDIN)|(1<<SCKL);//(1<<SCE);		// out: DATA (MOSI),CLCK, SCE

	DDRB|=(1<<PB2);				// SS has to be output!! If not our master will turn slave...

	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)/*|(1<<SPR1)*/;				// SPI enable, Master, Clock fcpu/16

	ms_delay(15);

	PORTD&=~(1<<RES);
	ms_delay(1);
	PORTD|=(1<<RES);									// Reset is set.
	PORTD&=~(1<<SCE);
	ms_delay(1);
	PORTD|=(1<<SCE);									// CE is set

	ms_delay(1);

						/* Initialization Commands */
	lcd_write_byte(CMD, 0x21 );		// LCD Extended Commands.
	lcd_write_byte(CMD, 0xc8 );		// set bias voltage
	lcd_write_byte(CMD, 0x06 );		// set temp correction
	lcd_write_byte(CMD, 0x13 );		// mode 1:48
	lcd_write_byte(CMD, 0x20 );		// use bias cmd
	lcd_clear();
	lcd_write_byte(CMD, 0x0c );		// set LCD mode: d = invert c=normal

	PORTD&=~(1<<SCE);				// clear SCE
	ms_delay(1);
}

					/* Command to check if it works */
void lcd_dot(void)
{
	lcd_write_byte(IMG,0x00);
	lcd_write_byte(IMG,0x3C);
	lcd_write_byte(IMG,0x7E);
	lcd_write_byte(IMG,0x7E);
	lcd_write_byte(IMG,0xFC);
	lcd_write_byte(IMG,0xF8);
	lcd_write_byte(IMG,0xFC);
	lcd_write_byte(IMG,0x7E);
	lcd_write_byte(IMG,0x7E);
	lcd_write_byte(IMG,0x3C);
	lcd_write_byte(IMG,0x00);
}

void lcd_char(char character)
{
	lcd_write_byte(IMG,0x00);
	for(int index=0;index<5	;index++) lcd_write_byte(IMG,ASCII[character-0x20][index]);
	lcd_write_byte(IMG,0x00);
}

void lcd_string(char *stringi)
{
	while(*stringi) lcd_char(*stringi++);
}

void lcd_place(uint8_t x,uint8_t y)
{
	lcd_write_byte(CMD,0x80|x);
	lcd_write_byte(CMD,0x40|y);
}

void lcd_clear(void)
{
	lcd_write_byte(CMD,0x0c);
	lcd_write_byte(CMD,0x80);
	for(unsigned int index=0;index<504;index++) lcd_write_byte(IMG,0x00);
}

void lcd_int(uint16_t a)
{
	if(a/10) lcd_int(a/10);
	lcd_write_byte(IMG,0x00);
	for(int index=0;index<5	;index++) lcd_write_byte(IMG,ASCII[(a%10)-0x20+'0'][index]);
	lcd_write_byte(IMG,0x00);
}

void lcd_bin(uint8_t hexal)
{
	uint8_t a=7;
	lcd_string("b: ");
	for(;a!=0;a--)	lcd_int((hexal&(1<<a))/(1<<a));
	lcd_int((hexal&(1<<a))/(1<<a));
}
