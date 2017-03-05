#include <QtCore>
#include <QThread>

#include <QDebug>
#include <QTimer>
#include <libusb-1.0/libusb.h>
#include "ComPoll.h"
#include "AVR_Code/usbdrv/usbconfig.h"

namespace 
{
libusb_hotplug_callback_handle handle;
static libusb_device_handle *usb_handle = NULL;
struct libusb_device_descriptor desc;
}

ComWorker &ComWorker::instance() 
{
    static ComWorker comWorker;
    return comWorker;
}

int hotplug_callback( struct libusb_context *ctx, struct libusb_device *dev, libusb_hotplug_event event, void *user_data) {
	int rc;
	(void)libusb_get_device_descriptor(dev, &desc);
	if (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == event) {
		rc = libusb_open(dev, &usb_handle);
		if (LIBUSB_SUCCESS != rc) {
			emit message(QStringList(QString("Could not open USB device\n")));
		}
	} else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event) {
		if (usb_handle) {
			libusb_close(usb_handle);
			usb_handle = NULL;
            comWorker.timer.stop();
            emit message(QStringList(QString("Device disconnected - stopped action")));
		}
	} else {
		emit message(QStringList(QString("Unhandled event %1s\n").arg( event)));
	}
	return 0;
}

/// Uses AVR_Code VID & Device ID to connect to the device
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

/// Set time how long moc3051 or some different should be open
/// TODO
/// get propper values from controllers - timescalled by n*(1/timebase_)
/// set controler controll data on/off
/// send/receive data
void ComWorker::onTimeout() {
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
    } else {
		emit message(QStringList(QString("Cant send! Error: %1s\n").arg( "No USB or regulation" )));
    }
}

void ComWorker::commRun()
{
    if ( usb_handle != 0 ) {
        connect(&comWorker.timer, SIGNAL(timeout()), &comWorker , SLOT(onTimeout()));
        comWorker.timer.start(comWorker.timebase);
    }
}
