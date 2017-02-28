HID_PID
=======

Temperature controller based on VUSB device  
→ Right now it works with simple temperature curve used to solidering, external data curve (needed to load or either loaded automaticly from file where executable is) PID is yet to be added.  
→ It's goal it to create usable, minimal temperature controller and soft.  
→ Mesure ments are usable now, tested only on old chineese pecos clone.  
→ It's dedicated for heat controll purposes.  
→ Electronics design will be posted in meantime  
→ To be yet added PID functionality - it's in name but it occured to me that bistable type of setup works suprisingly well, so it's far on the to do list.  
→ USB and global values shall be packed in class.  

TODO:  
* all setups to one cpp/h file ( faster to edit and these are small righ now... )  
* end USB device usage refactoring 
* windows scaling doesnt work now - this shall be added
* TODO PID
* TODO average value from TEMP curve between points
* TODO check if free running ADC might be a bit better
* TODO in and out transfer instead of two transfers
 
Looks
========
![selection](https://raw.githubusercontent.com/pholat/HID_PID/master/in_work1.png)
![selection](https://raw.githubusercontent.com/pholat/HID_PID/master/in_work2.png)
![chart](https://raw.githubusercontent.com/pholat/HID_PID/master/itsworking.png)

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
It's written for linux, but there shall be no problem to port it to windows.
I don't take any responsibility for how you use it.
