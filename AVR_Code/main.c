/*
 * main.c
 *
 *  Created on: 18-11-2013
 *      Author: pholat
 *
 *      It's Bistate controller right now - as it works really nice.
 *      Next to do is to add flag for choosing type of work
 *      	→ Bistate
 *      	→ Tristate
 *      	→ PID
 *
 */

//////////////// includes		/////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "usbdrv/usbdrv.h"
#include <util/delay.h>
#include "adc.h"
#include "common.h"

uint8_t volatile flag = 0;
//////////////// COMMAND NR ON usbFunctionSetup /////////
enum bufferByte {
    Flag,TempYoungADC,TempOldADC,TempYoungSet,TempOldSet,PID_P,PID_I,PID_D
};

volatile uint8_t replyBuf[USB_DATA_SIZE]= {0,0,0,0,0,0,0,0};
static uchar dataReceived = 0, dataLength = 0; // for USB_DATA_IN

union TwoBytes {
    uint16_t u16;
    uint8_t u8[2];
} TempSet;

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
USB_PUBLIC uchar usbFunctionWrite(uchar *data, uchar len)
{
    uchar i;

    for(i = 0; dataReceived < dataLength && i < len; i++, dataReceived++) {
        // I do not want to write to ADC data which can be written before or after...
        if(i!=TempOldADC || i!=TempYoungADC) replyBuf[dataReceived] = data[i];
    }
    return (dataReceived == dataLength); // 1 if we received it all, 0 if not
}


int main()
{
    replyBuf[TempOldSet]=0x03;
    replyBuf[TempYoungSet]=0xFF;

    DDRC|=(1<<PC5);
    PORTC|=(1<<PC5);

    DDRC|=(1<<PC1)|(1<<PC2)|(1<<PC3)|(1<<PC4);
    PORTC&=~(1<<PC2);
    PORTC|=(1<<PC1);
    // ADC on ADC0, On, start, (full 1024 resolution), /*adc interrupt enable*/
    // Aref - internall Vref turned off
    adc_on(0|(1<<REFS0),(1<<ADEN)|(1<<ADSC)/*|(1<<ADIE)*/);
    wdt_enable(WDTO_1S); // enable 1s watchdog timer
    usbInit();
    usbDeviceDisconnect(); // enforce re-enumeration
    for(i = 0; i<250; i++) { // wait 500 ms
        wdt_reset(); // keep the watchdog happy
        _delay_ms(2);
    }
    usbDeviceConnect();
    sei(); // Enable interrupts after re-enumeration
    uint16_t counter=1;

    while(1) {
        wdt_reset(); // keep the watchdog happy
        uint16_t inUseADCval=adc_one(0);
        // If you send TempOldSet / Temps Young set it's ping: data which you send from PC
        // shall be back at you and both plots shall be the same.
        replyBuf[TempOldADC]=(ADC>>8)&0x03;
        replyBuf[TempYoungADC]=(ADC&0xFF);//ADCL
        if((counter==1)) {
            TempSet.u8[1]=replyBuf[TempOldSet];
            TempSet.u8[0]=replyBuf[TempYoungSet];
            if(inUseADCval>TempSet.u16) PORTC|=(1<<PC4);
            else {
                PORTC&=~(1<<PC4);
            }
            counter=250;
        }
        counter--;
        _delay_ms(1);

        usbPoll();
    }
    return 0;
}
