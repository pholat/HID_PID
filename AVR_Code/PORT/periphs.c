#include "periphs.h"

void Init(void)
{ //  pid_Init(K_P, K_I, K_D, &pidData);

  // Set up timer, enable timer/counte 0 overflow interrupt
  // Muszę poprawić - bo zmieniłem procka - więc zmienił sie rejestr.
  TCCR1B = /*(1<<CS10)|(1<<CS11)|*/ (1<<CS12);	// Prescaller
  TIMSK = (1<<TOIE0);
  TCNT0 = 0;
}

int16_t Get_Measurement(void)
{
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	return ADC;
}

void adc_on( uint8_t input )
{
	ADMUX|=(1<<REFS0);											// internal reference voltage
	switch(input)
	{															// choose your output
		case 0: ADMUX&=~((1<<MUX1)|(1<<MUX0)); break;				// PB5
		case 1: ADMUX&=~(1<<MUX1); ADMUX|=(1<<MUX0); break;		// PB2
		case 2: ADMUX|=(1<<MUX1); ADMUX&=~(1<<MUX0); break;		// PB4
		case 3: ADMUX|=(1<<MUX1)|(1<<MUX0); break;					// PB3
	}
//	ADMUX|=(1<<ADLAR);											// 10 bit in use
	ADCSRA|=(1<<ADEN);											// ADC enable
	ADCSRA|=(1<<ADPS1);											// Multiplexer 4 - ale zmienic trzeba do 8mhz
}

void pwm_on()
{
	/* Timer 2 daje zegar tykający co 1s
	 * Tryb: CTC
	 * Prescaler=32
	 * OCR2=250
	 * Tykniecia = (OCR2*Prescaler/8M)
	 * Interrupt on overflow
	 * */
	TCCR2|=(1<<WGM21);
	TCCR2|=(1<<CS21)|(1<<CS20);
	TIMSK|=(1<<OCIE2);
	OCR2=250;
}

/*
 * Nie było potrzebne
void clock_on()
{
	TCCR1B|=(1<<WGM12); 			// Timer jako CTC
	TCCR1B|=(1<<CS11)|(1<<CS10);	// Podział przez 64
	OCR1A=125;						// Przerwanie co 0,001s == ((8Mhz/prescaller)^-1)*(125)
	TIMSK|=(1<<OCIE1A);				// out on compare.
}*/
