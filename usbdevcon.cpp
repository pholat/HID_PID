#include "usbdevcon.h"

UsbDevCon::UsbDevCon(int in_dev_number)
{
    for(int i=0; i<8; i++) {
        buffer[i]=0;
    }
    dev_number=in_dev_number;
    if(0 > libusb_open(device[dev_number],&device_handle)) dev_open = -1;
    else {
        dev_open=1;
        imanufacturer = libusb_get_string_descriptor_ascii(device_handle,device_descriptor.iManufacturer,string_buffer_manufacturer,sizeof(string_buffer_manufacturer));
        if(0 > libusb_get_string_descriptor_ascii(device_handle,device_descriptor.iProduct,string_buffer_product,sizeof(string_buffer_product)) ) desc_to_asci=-1;
        else {
            desc_to_asci = 1;
            //converting the string to const char*
            cc_manufacturer = (const char*)string_buffer_manufacturer;
            manufacturer = cc_manufacturer;
            //converting the string to const char*
            cc_product = (const char*)string_buffer_product;
            product = cc_product;
        }
    }
}

UsbDevCon::~UsbDevCon()
{
    libusb_close(device_handle);
}
