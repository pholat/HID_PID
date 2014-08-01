#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <libusb.h>

#include "qwt_plot_curve.h"
#include "qvector.h"

double pwm, pwm_min, pwm_max;
double temp,temp_min, temp_max;
double prop, prop_min, prop_maxp;
double integ, integ_min, integ_max;
double deriv, deriv_min, deriv_max;
int usb_number;

//libUSB members
libusb_device_handle *device_handle; //handle to USB device
libusb_device_descriptor device_descriptor; //the device descriptor
libusb_device  **device_list; //to store all found USB devices
ssize_t device_count; //holding number of devices in list
int r; //for return values
libusb_context *ctx; //a libusb context for library intialization
int USB_Flag_conected=0;
uchar buffer[8]={0,0,0,0,0,0,0};
enum bufferByte{
    Flag,TempYoungADC,TempOldADC,TempYoungSet,TempOldSet,PID_P,PID_I,PID_D
    };

//Plot data
double dupa=100;
double hupa=4;
double timeSecs=1;
QVector<double> PlotTempData(QVector<double>(100));
QVector<double> PlotTempSet(QVector<double>(100));
QVector<double> PlotTime(QVector<double>(100));

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    // Setting temperature to 20*C
    tempSet(20)
{
    ui->setupUi(this);
    setWindowTitle("Fila Controll");
// Set USB flag byte clear
    buffer[0]=0;

    // PLOT Init
    CurvePlotTempData = new QwtPlotCurve("Real 1");
    CurvePlotTempSet = new QwtPlotCurve("Set 2");

    //PLOT setup
    ui->qwtPlot->setTitle("Set and real temp");
    ui->qwtPlot->setAxisTitle( QwtPlot::yLeft ,"Temp [*C]");
    ui->qwtPlot->setAxisTitle( QwtPlot::xBottom, "Time [s] ");
//    ui->qwtPlot->set
//    PlotTempData.insert(PlotTempData.size(),dupa);
//    PlotTime.insert(PlotTime.size(),siupa);
//    PlotTempSet.insert(PlotTempSet.size(),hupa);
    CurvePlotTempData->setSamples(PlotTime,PlotTempData);
    CurvePlotTempData->setPen( QPen(Qt::red));
    CurvePlotTempSet->setSamples(PlotTime,PlotTempSet);
    CurvePlotTempSet->setPen( QPen(Qt::blue));
    CurvePlotTempData->attach(ui->qwtPlot);
    CurvePlotTempSet->attach(ui->qwtPlot);

    //USB setup
    ui->dial_Temp->setMaximum(250);
    ui->dSpinBox_Temp->setMaximum(250);
    ui->progressBar->setValue(0);
    ui->dial_PWM->setMaximum(100);
    ui->dSpinBox_PWM->setMaximum(100);

    ui->qwtPlot->replot();
    // ADDED for timer
    // Moved to on send button clicked
//    timerId = startTimer(1000);

    // USB init
    r = libusb_init(&ctx); //initializing the library
    //detecting all the connected devices
    device_count = libusb_get_device_list(ctx, &device_list);
    usableDevCount = new int(device_count);
}

MainWindow::~MainWindow()
{
    libusb_close(device_handle);
    killTimer(timerId);
    delete usableDevCount;
    delete ui;
}



void MainWindow::on_dial_PWM_sliderMoved(int position)
{
    pwm=position;
    ui->dSpinBox_PWM->setValue(pwm);
}
void MainWindow::on_dial_Temp_sliderMoved(int position)
{
    temp=position;
    ui->dSpinBox_Temp->setValue(temp);
}

void MainWindow::on_verticalSlider_P_sliderMoved(int position)
{
    prop=position;
    ui->dSpinBox_P->setValue(prop);
}
void MainWindow::on_verticalSlider_P_valueChanged(int value)
{
    prop=value;
    ui->dSpinBox_P->setValue(prop);

}
void MainWindow::on_dSpinBox_P_valueChanged(double arg1)
{
    prop=arg1;
    ui->verticalSlider_P->setSliderPosition(int(arg1));
}

void MainWindow::on_verticalSlider_I_sliderMoved(int position)
{
    integ=position;
    ui->dSpinBox_I->setValue(integ);
}
void MainWindow::on_verticalSlider_I_valueChanged(int value)
{
    integ=value;
    ui->dSpinBox_I->setValue(integ);
}
void MainWindow::on_dSpinBox_I_valueChanged(double arg1)
{
    integ=arg1;
    ui->verticalSlider_I->setValue(integ);
}

void MainWindow::on_verticalSlider_D_sliderMoved(int position)
{
    deriv=position;
    ui->dSpinBox_D->setValue(deriv);
}
void MainWindow::on_verticalSlider_D_valueChanged(int value)
{
    deriv=value;
    ui->dSpinBox_D->setValue(deriv);
}
void MainWindow::on_dSpinBox_D_valueChanged(double arg1)
{
    deriv=arg1;
    ui->verticalSlider_D->setValue(deriv);
}

//------------------------------ USB controll

void MainWindow::listview_populate_usb_devices()
{
    ui->listWidget->clear(); //clearing the list view

    ssize_t i; //for the device iterator for loop
    QString list_entry; //for using library outputs in char * in Qt framework

    int progress_bar_value = 0;
    int progress_bar_correction = 0;
    char device_open = 0; //for checking whether a device was opened or not
    unsigned char string_buffer_manufacturer[4096]; //for storing manufacturer descriptor
    unsigned char string_buffer_product[4096]; //for storing product descriptor
    const char *cc_manufacturer; //for the casting to use the string output from libusb in Qt
    const char *cc_product; //for the casting to use the string output from libusb in Qt

    //inform error on status bar
    if(r < 0) ui->statusBar->showMessage("Error: Initializing libusb");

    else
    {

        //inform error on status bar
        if(device_count <= 0) ui->statusBar->showMessage("Info: No device found");

        else
        {
            //resetting the progress bar
            ui->progressBar->reset();

            //for loop iterating through found devices
            for(i=0;i<device_count;i++)
            {
                //getting device descriptor
                r = libusb_get_device_descriptor(device_list[i], &device_descriptor);

                //inform error on status bar
                if(r < 0)   ui->statusBar->showMessage("Error: Failed to get device descriptor");

                else
                {
                    //opening the device
                    r = libusb_open(device_list[i],&device_handle);

                    if(r < 0)
                    {
                        //inform error on status bar
                        ui->statusBar->showMessage("Error: Opening USB device");
                        //indicating that device is not open
                        device_open = 0;
                    }

                    else
                    {
                        //indicating that device is open
                        device_open = 1;

                        // local int for counting usable devices;
                        // alternaticve is to use list or vector with UsableDevCount
                        static int LocalUsableDevCounter=0;
                        //setting opened dev number in usable_dev_count as usable device
                        *(usableDevCount+LocalUsableDevCounter)=i;
                        // LocalUsableDevCounter needs to be num of next prey
                        LocalUsableDevCounter++;

                        //getting the ASCII text value from the descriptor field
                        r = libusb_get_string_descriptor_ascii(device_handle,device_descriptor.iManufacturer,string_buffer_manufacturer,sizeof(string_buffer_manufacturer));

                        if(r < 0)
                        {
                            //reporting error on the list entry
                            ui->listWidget->addItem("Error: Converting descriptor to ASCII [iManufacturer]");
                            ui->listWidget->item(ui->listWidget->count() - 1)->setForeground(Qt::white);
                            ui->listWidget->item(ui->listWidget->count() - 1)->setBackground(Qt::red);
                        }

                        else
                        {
                            //getting the ASCII text value from the descriptor field
                            r = libusb_get_string_descriptor_ascii(device_handle,device_descriptor.iProduct,string_buffer_product,sizeof(string_buffer_product));

                            if(r < 0)
                            {
                                //reporting error on the list entry
                                ui->listWidget->addItem("Error: Converting descriptor to ASCII [iProduct]");
                                ui->listWidget->item(ui->listWidget->count() - 1)->setForeground(Qt::white);
                                ui->listWidget->item(ui->listWidget->count() - 1)->setBackground(Qt::red);
                            }

                            else
                            {
                                //converting the string to const char*
                                cc_manufacturer = (const char*)string_buffer_manufacturer;
                                QString manufacturer(cc_manufacturer);

                                //converting the string to const char*
                                cc_product = (const char*)string_buffer_product;
                                QString product(cc_product);

                                //constructing the string for list entry
                                list_entry = manufacturer + " " + product;

                                ui->listWidget->horizontalScrollBar();
                                ui->listWidget->verticalScrollBar();
                                ui->listWidget->addItem(list_entry); //adding the list entry

                                //closing opened USB device
                                if (device_open == 1)
                                {
                                    //closing the deivce
                                    libusb_close(device_handle);
                                    //indicating that device is open
                                    device_open = 0;
                                }

                                //calculate and set the progress bar value
                                progress_bar_value = (i+1) * (100/device_count);

                                //correcting value
                                if ( (i+1) == device_count)
                                {
                                    progress_bar_correction = 100 - progress_bar_value;
                                    progress_bar_value = progress_bar_value + progress_bar_correction;
                                }

                                //setting the value
                                ui->progressBar->setValue(progress_bar_value);
                            }
                        }
                    }
                }
            }
            //closing opened USB device
            if (device_open == 1)
            {
                //closing the deivce
                libusb_close(device_handle);
                //indicating that device is open
                device_open = 0;
            }
        }
    }
}

void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    usb_number=index.row();
    ui->spinBox_DevNum->setValue(*(usableDevCount+usb_number));
}


void MainWindow::on_pushButton_link_clicked()
{
    USB_Flag_conected=1;
    int usb_fail_flag=libusb_open(device_list[*(usableDevCount+usb_number)],&device_handle);
    if (usb_fail_flag<0)
    {
        ui->textBrowser_usbMessage->setTextBackgroundColor(Qt::red);
        ui->textBrowser_usbMessage->setText("Connection fali");
    }
    else
    {
        ui->textBrowser_usbMessage->setTextBackgroundColor(Qt::green);
        ui->textBrowser_usbMessage->setText("Connected!");
    }
}

void MainWindow::on_spinBox_DevNum_valueChanged(int arg1)
{
    ui->textBrowser_usbMessage->setTextBackgroundColor(Qt::red);
    ui->textBrowser_usbMessage->setText("Bad Kitty!");
}

//ADDED for timer
void MainWindow::timerEvent(QTimerEvent *event)
{
    /*
     * This part was to check if Timer event is working :)
     * → If it's working, than each 1 sec you see Timeout or Wazzaaa
     *
    static int i=0;
    if(i==0)
    {
        ui->textBrowser_sendMessage->setText("Timeout!");
        i++;
    }else{
        i=0;
        ui->textBrowser_sendMessage->setText("Wazzaaa!");
    }*/
    /* Next part of tests, it's better to do it step by step
     * This part is actually nearly done */
    /*
    static double insertion=20, i=0;
    if(i<341)
    {
        if(i<80) tempSet++;
        else if(i<140) tempSet=100;
        else if(i<280) tempSet++;
        else if(i<340) tempSet--;
        i++;
        PlotTempData.insert(PlotTempData.size(),tempSet);
        PlotTime.insert(PlotTime.size(),siupa);
        PlotTempSet.insert(PlotTempSet.size(),hupa+insertion);
        insertion+=1;
        siupa+=1;
        CurvePlotTempData->setSamples(PlotTime,PlotTempData);
        CurvePlotTempData->setPen( QPen(Qt::red));
        CurvePlotTempSet->setSamples(PlotTime,PlotTempSet);
        CurvePlotTempSet->setPen( QPen(Qt::blue));
        ui->qwtPlot->replot();
    }*/
    if((buffer[Flag]==1) && (USB_Flag_conected==1))
    {
        static double i=0;
        if(i<341)
        {
            if(i<80) tempSet++;
            else if(i<140) tempSet=100;
            else if(i<280) tempSet++;
            else if(i<340) tempSet--;
            i++;
            // Plot set temperature
            PlotTempData.insert(PlotTempData.size(),tempSet);
            // Plot flown fime
            PlotTime.insert(PlotTime.size(),timeSecs);
            // Plot aquired temperature
            PlotTempSet.insert(PlotTempSet.size(),(double)(buffer[TempYoungADC]|(buffer[TempOldADC]<<8)));
            timeSecs+=1;
            // Normal ploting procedure :)
            CurvePlotTempData->setSamples(PlotTime,PlotTempData);
            CurvePlotTempData->setPen( QPen(Qt::red));
            CurvePlotTempSet->setSamples(PlotTime,PlotTempSet);
            CurvePlotTempSet->setPen( QPen(Qt::blue));
            ui->qwtPlot->replot();
        }
    }else{
        ui->textBrowser_usbMessage->setText("no USB, no plot");
    }
}

void MainWindow::on_pushButton_send_clicked()
{

    // TODO in and out transfer
    // than add "start plotting"
    // then maybe send rest of data
    // and finally program will be ready to test
    buffer[Flag]=1;
    // ADDED for timer start
    // Shall be moved to else part below
    timerId = startTimer(1000);
    if(USB_Flag_conected==0)
    {
        ui->textBrowser_usbMessage->setText("Error - no USB connected");
    }
    else
    {
        // Write data to send - in use use data table → than tempSet
        //                    - in use buttons and sliders → then temp
        //                    - and so one... "make choice" button shall be added
        // Grab LSB → Grab MSB → Set PID values (double to uint conversion!)
        buffer[TempYoungSet]=(u_int8_t)tempSet&0xFF;
        buffer[TempOldSet]  =((u_int8_t)tempSet>>8);
        buffer[PID_P]=(u_int8_t)prop;
        buffer[PID_I]=(u_int8_t)integ;
        buffer[PID_D]=(u_int8_t)deriv;

        // First of all send data
        libusb_control_transfer(device_handle,LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT,
                                5 , 0, 0, buffer, sizeof(buffer), 5000);
        // Secondly grab data
        libusb_control_transfer(device_handle,LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT,
                                5 , 0, 0, buffer, sizeof(buffer), 5000);
    }
}
