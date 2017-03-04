#include <QtCore>
#include <QThread>

#include <QDebug>
#include <QTimer>
#include <libusb-1.0/libusb.h>
#include "ComPoll.h"
#include "AVR_Code/usbdrv/usbconfig.h"

// TODO create it in mainwindow.cpp and pass callbacks :)
namespace 
{
ComWorker comWorker;
libusb_hotplug_callback_handle handle;
static libusb_device_handle *usb_handle = NULL;
struct libusb_device_descriptor desc;
}

/// Device plugged/unplugged callback - TODO add Signal/slot for these two!
int hotplug_callback( struct libusb_context *ctx, struct libusb_device *dev, libusb_hotplug_event event, void *user_data) {
	int rc;
	(void)libusb_get_device_descriptor(dev, &desc);
	if (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == event) {
		rc = libusb_open(dev, &usb_handle);
		if (LIBUSB_SUCCESS != rc) {
			printf("Could not open USB device\n");
		}
	} else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event) {
		if (usb_handle) {
			libusb_close(usb_handle);
			usb_handle = NULL;
		}
	} else {
		printf("Unhandled event %d\n", event);
	}
	return 0;
}

// TODO connect to the device if it was already plugged....
/// Uses AVR_Code VID & Device ID
ComWorker::ComWorker() : timebase(100), usbConStatus(false)
{
	libusb_init(NULL);
	int rc = libusb_hotplug_register_callback( NULL, 
        LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, 
        0,
        USB_CFG_VENDOR_ID, USB_CFG_DEVICE_ID,
		LIBUSB_HOTPLUG_MATCH_ANY, 
        hotplug_callback, 
        NULL,
		&handle);
	if (LIBUSB_SUCCESS != rc) {
		qDebug() << "Error creating a hotplug callback\n";
		libusb_exit(NULL);
	}
	libusb_device_handle* tmp =  libusb_open_device_with_vid_pid 	( 	NULL,
        USB_CFG_VENDOR_ID, USB_CFG_DEVICE_ID
	);
	if ( tmp ) 
	{
		usb_handle = tmp;
	}
}

ComWorker::~ComWorker() 
{
  libusb_hotplug_deregister_callback(NULL, handle);
  libusb_exit(NULL);
}

bool ComWorker::usbConnected() 
{
     return usbConStatus;
}

/// Set time how long moc should be open
void ComWorker::onTimeout() {
    if ( usbConnected() ) {
    }
//	// get propper values from controllers - timescalled by n*(1/timebase_)
//	// set controler controll data on/off
//	// send/receive data
//  // MOVED from mainvindow.cpp
/// Write data to send - in use use data table → than tempSet
///                    - in use buttons and sliders → then temp
///                    - and so one... "make choice" button shall be added
/// Grab LSB → Grab MSB → Set PID values (double to uint conversion!)
/// and finally program will be ready to test
/// buffer[Flag]=1;
/// Calculate temp to set

    if( (usb_handle!=0) && (buffer[Flag]==1) && (RegulationType!=0)) {
		libusb_control_transfer(
				usb_handle,
				LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
				request ,
				0,
				0,
				data, 
				size,
				timeout );
//
//        double tempToSet = RegulationType->returnTemp(TimmingValue++,temp);
//
//        u_int16_t temp_to_device = temp_to_send(tempToSet);
//        buffer[TempYoungSet]=temp_to_device&0xFF;
//        buffer[TempOldSet]  =(temp_to_device>>8)&0xFF;
//        buffer[PID_P]=(u_int8_t)prop;
//        buffer[PID_I]=(u_int8_t)integ;
//        buffer[PID_D]=(u_int8_t)deriv;
//
//        usbDev->control_transfer(Endpoint::Direction::Out, USB_DATA_IN, buffer, bufsize );
//        usbDev->control_transfer(Endpoint::Direction::In, USB_DATA_OUT, buffer, bufsize );
//        plotChart(tempToSet, timeSecs++, temp_find( buffer[TempYoungADC] | (buffer[TempOldADC]<<8) ) );
//
//        qDebug() << "ADC  val: " << ( buffer[TempYoungADC] | (buffer[TempOldADC]<<8) );
//        qDebug() << "TEMP val: " << temp_find( buffer[TempYoungADC] | (buffer[TempOldADC]<<8) );
//    } else {
//        if( usbDev == 0 ) ui->textBrowserLOG->addItem("no USB, no plot");
//        if(RegulationType==0) ui->textBrowserLOG->addItem("No regulation");
    }
}

void commRun()
{
	connect(&comWorker.timer, SIGNAL(timeout()), &comWorker , SLOT(onTimeout()));
	comWorker.timer.start(comWorker.timebase);
}
