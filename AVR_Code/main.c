/*
 * main.c
 *
 *  Created on: 18-11-2013
 *      Author: pholat
 */

//////////////// includes		/////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "usbdrv/usbdrv.h"
#include <util/delay.h>
#include "adc.h"
#include "spi.h"
#include "n5110.h"

//////////////// Flaga dla nRF
#define DATA_READY (1<<0)
#define ONE_SECOND (1<<1)
#define NEW_DATA 	(1<<2)
uint8_t volatile flag = 0;
#define USB_DATA_SIZE 8		// Its a size of data table.
//////////////// COMMAND NR ON usbFunctionSetup /////////

#define USB_DATA_OUT 		2	//Device to PC
#define USB_DATA_IN 		4	//Fine PC to Device

enum bufferByte{
    Flag,TempYoungADC,TempOldADC,TempYoungSet,TempOldSet,PID_P,PID_I,PID_D
    };

volatile uint8_t replyBuf[USB_DATA_SIZE]={0,0,0,0,0,0,0,0};
static uchar dataReceived = 0, dataLength = 0; // for USB_DATA_IN

// this gets called when custom control message is received
USB_PUBLIC uchar usbFunctionSetup(uchar *data)
{
	usbRequest_t *rq = (void *)data; // cast data to correct type

	switch(rq->bRequest) { // custom command is in the bRequest field
/* Thats a way to reply data back to PC
 * With that what's inside will be replied to PC
 * PC program have to have USB_DATA_SIZE uint8t big
 * place for it
 * Max data to send can be 255 bytes.
 */
    case USB_DATA_OUT:
        usbMsgPtr = replyBuf;
        return sizeof(replyBuf);

/* With that data can be received.
 * causes call for usbFunctionWrite
 * Wchich will result in stocking data in replyBuf till
 * the end of len
 */
    case USB_DATA_IN:
		dataLength  = (uchar)rq->wLength.word;
        dataReceived = 0;

		if(dataLength  > sizeof(replyBuf)) // limit to buffer size
			dataLength  = sizeof(replyBuf);
		return USB_NO_MSG; // usbFunctionWrite will be called now
	}

    return 0; // should not get here
}

// This gets called when data is sent from PC to the device
USB_PUBLIC uchar usbFunctionWrite(uchar *data, uchar len) {
	uchar i;

	for(i = 0; dataReceived < dataLength && i < len; i++, dataReceived++)
		{
		// I do not want to write to ADC data which can be written before or after...
			if(i!=TempOldADC || i!=TempYoungADC) replyBuf[dataReceived] = data[i];
		}
    return (dataReceived == dataLength); // 1 if we received it all, 0 if not
}


int main() {
	uchar i;
	DDRC&=~(1<<PC5);
	PORTC&=~(1<<PC5);
	DDRC|=(1<<PC1)|(1<<PC2)|(1<<PC3)|(1<<PC4);
	PORTC&=~(1<<PC2);
	PORTC|=(1<<PC1);
	// ADC on ADC0, On, start, (full 1024 resolution), /*adc interrupt enable*/
	// Aref - internall Vref turned off
	adc_on(5|(1<<REFS0),(1<<ADEN)|(1<<ADSC)/*|(1<<ADIE)*/);
	//spi_init();
	//lcd_init();
    wdt_enable(WDTO_1S); // enable 1s watchdog timer
    usbInit();
    usbDeviceDisconnect(); // enforce re-enumeration
    for(i = 0; i<250; i++) { // wait 500 ms
        wdt_reset(); // keep the watchdog happy
        _delay_ms(2);
   }
    usbDeviceConnect();
    sei(); // Enable interrupts after re-enumeration

    while(1)
    {
        wdt_reset(); // keep the watchdog happy
        adc_one(5);
        replyBuf[TempOldADC]=(ADC>>8)&0x03;
        replyBuf[TempYoungADC]=(ADC&0xFF);//ADCL;
/*        lcd_clear();
        lcd_place(0,0);
        lcd_int(ADCH);
        lcd_place(0,1);
        lcd_int(ADCL);
        lcd_place(0,2);
        lcd_int(ADC);
        lcd_place(0,3);
        lcd_int(ADC&0xFF);
        lcd_place(0,4);
        lcd_int((ADC&0xFF00)>>8);
        */
        usbPoll();
    }
    return 0;
}
