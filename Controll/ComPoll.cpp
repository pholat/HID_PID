#include <QtCore>
#include <QThread>

#include <QDebug>
#include <QTimer>
#include "ComPoll.h"
#include "AVR_Code/usbdrv/usbconfig.h"
#include "AVR_Code/common.h"

namespace 
{
libusb_hotplug_callback_handle handle;
static libusb_device_handle *usb_handle = NULL;
struct libusb_device_descriptor desc;
// TODO this is very temporary...
char vid[2] = { (char)USB_CFG_VENDOR_ID };
char pid[2] = { (char)USB_CFG_DEVICE_ID };
int VID = vid[0] + ((int)vid[1]<<8);
int PID = pid[0] + ((int)pid[1]<<8);
};

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
            emit ComWorker::instance().message(QStringList(QString("Could not open USB device\n")));
		}
	} else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event) {
		if (usb_handle) {
			libusb_close(usb_handle);
			usb_handle = NULL;
            ComWorker::instance().timer.stop();
            emit ComWorker::instance().message(QStringList(QString("Device disconnected - stopped action")));
		}
	} else {
        emit ComWorker::instance().message(QStringList(QString("Unhandled event %1s\n").arg( event)));
	}
	return 0;
}

/// Uses AVR_Code VID & Device ID to connect to the device
ComWorker::ComWorker() : timebase(100), usbConStatus(false)
{
	libusb_init(NULL);
	int rc = libusb_hotplug_register_callback( NULL, 
        (libusb_hotplug_event)(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT),
        (libusb_hotplug_flag)0,
        PID, VID,
		LIBUSB_HOTPLUG_MATCH_ANY, 
        hotplug_callback, 
        NULL,
        &handle);
	if (LIBUSB_SUCCESS != rc) {
		qDebug() << "Error creating a hotplug callback\n";
		libusb_exit(NULL);
	}
	libusb_device_handle* tmp =  libusb_open_device_with_vid_pid 	( 	NULL,
        VID, PID
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
/// REMOVE TEMOPORARY DUMMY STUFF in transfer
void ComWorker::onTimeout() {
    unsigned char buffer[8];
    if( (usb_handle!=0) ) { // && (buffer[Flag]==1) && (RegulationType!=0)) {
		libusb_control_transfer(
				usb_handle,
				LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
                USB_DATA_OUT ,
				0,
				0,
                buffer,
                8,
                100 );
    } else {
		emit message(QStringList(QString("Cant send! Error: %1s\n").arg( "No USB or regulation" )));
    }
}

void ComWorker::commRun()
{
    if ( usb_handle != 0 ) {
        connect(&timer, SIGNAL(timeout()), this , SLOT(onTimeout()));
        timer.start(timebase);
    }
}
