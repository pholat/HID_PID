HID_PID
=======

Temperature controller based on VUSB device  
→ Right now it works with simple temperature curve used to solidering, external data curve (needed to load or either loaded automaticly from file where executable is) PID is yet to be added.  
→ It's goal it to create usable, minimal temperature controller and soft.  
→ It's dedicated for heat controll purposes.  
→ Please note that controll works only with VUSB device User and Product ID ( available in AVR_Code/usbdrv/usbconfig.h ) 

TODO:  
* change electronics to support partial sine ( 0 crossing detection for nice 0-100% 230V controll ) 
* TODO Support all controlls ( PID 3/4 done ) + adjust avr code
* TODO check if free running ADC might be a bit better
* TODO in and out transfer instead of two transfers
 
Looks
========
![selection](https://raw.githubusercontent.com/pholat/HID_PID/master/Doc/in_work1.png)
![selection](https://raw.githubusercontent.com/pholat/HID_PID/master/Doc/in_work2.png)
![chart](https://raw.githubusercontent.com/pholat/HID_PID/master/Doc/itsworking.png)

Qt app compilation
========
Please checkout with:
```
git pull --recurse-submodules
```
Working wersions marked with tag. To compile a working/compileable version checkout last tag. Right now it's:  
#### v0.1
App needs libusb-1.0 and qwt to compile, on Ubuntu it means you need to install:
```bash
sudo apt-get install libqwt-qt5-dev libusb-1.0-0-dev  
```

Final thoughts
=======
It's written on linux, but there shall be no problem to run on Windows.
I don't take any responsibility for how anybody uses it.
