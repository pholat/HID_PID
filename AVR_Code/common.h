#pragma once

#define USB_DATA_OUT 		2	//Device to PC
#define USB_DATA_IN 		4	//Fine PC to Device
#define USB_DATA_SIZE       8   // Its a size of data table.

enum bufferByte {
    Flag,TempYoungADC,TempOldADC,TempYoungSet,TempOldSet,PID_P,PID_I,PID_D
};
