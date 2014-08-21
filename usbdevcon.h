#ifndef USBSIMPLECONT_H
#define USBSIMPLECONT_H

#include <libusb.h>
#include <QString>

/* Future UsbDevCon class needed to connect device
 * It's needed so it would be called each time new dev will be checked,
 * also it's needed so that each time we select new usb old usb destructor
 * will be called and so it will be automaticly disconnected
 *
 * From this class shall inherit UsbDevList class or it shall include that class
 * The class I'm speaking off will initialise USB each time it's construced
 * and deinitialised each time destructor is called. It will store number of
 * accesibile devices so that that devices could be conneced with UsbDevCon
 * */

class UsbDevCon
{
public:
    UsbDevCon(int in_dev_number);
    ~UsbDevCon();
    void usbConect();
    // This function is to be provided next
    void sendHidData(u_int8_t* data_table,u_int8_t lenght);

private:
    int dev_number;
    int error_flag , dev_open, desc_to_asci, imanufacturer;
    QString manufacturer,product;
    libusb_device_handle *device_handle; //handle to USB device
    libusb_device_descriptor device_descriptor; //the device descriptor
    libusb_device  **device; //to store USB_Device
    libusb_context *ctx; //a libusb context for library intialization
    unsigned char string_buffer_manufacturer[4096]; //for storing manufacturer descriptor
    unsigned char string_buffer_product[4096]; //for storing product descriptor
    const char *cc_manufacturer; //for the casting to use the string output from libusb in Qt
    const char *cc_product; //for the casting to use the string output from libusb in Qt
    uchar buffer[8];//
};

#endif // USBSIMPLECONT_H
