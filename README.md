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


To compile
=======
 You will need:
 
 → Qt5
 
 → Qwt
 
 → libusb
 
 
Final thoughts
=======
It's written for linux, but there shall be no problem to port it to windows.
I don't take any responsibility for how you use it.
