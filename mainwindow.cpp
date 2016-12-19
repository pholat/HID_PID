#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <libusb-1.0/libusb.h>
#include <qwt/qwt_plot_curve.h>
#include <QVector>

#include "tempTable.h"

double pwm, pwm_min, pwm_max;
double temp,temp_min, temp_max;
double prop, prop_min, prop_maxp;
double integ, integ_min, integ_max;
double deriv, deriv_min, deriv_max;

//libUSB members - these tells us on what endpoints we work
#define USB_DATA_OUT 		2	//Device to PC
#define USB_DATA_IN 		4	//Fine PC to Device

// Maybe instead globals use unique pointer here for plot data and usb send / receive data
uchar buffer[8]= {0,0,0,0,0,0,0};
enum bufferByte {
    Flag,TempYoungADC,TempOldADC,TempYoungSet,TempOldSet,PID_P,PID_I,PID_D
};

// Plot data
double timeSecs=1;
QVector<double> PlotTempData(QVector<double>(100));
QVector<double> PlotTempSet(QVector<double>(100));
QVector<double> PlotTime(QVector<double>(100));

void ReportError( QListWidget *on, QString &str ) {
    on->addItem(str);
    on->item(on->count() - 1)->setForeground(Qt::white);
    on->item(on->count() - 1)->setBackground(Qt::red);
}

void printNonRootUSBDevs( QListWidget *listWidget, UsbContainer *usbContainer) {
    listWidget->clear();
    listWidget->addItems(usbContainer->listNonRootDevices());
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    usbcontainer(new UsbContainer(&usbErrorLog))
{
    ui->setupUi(this);
    // Setting temperature to 20*C
    TimmingValue=0;
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

    // USB initialised
    ui->textBrowserLOG->textMessage(usbErrorLog);
    printNonRootUSBDevs( ui->listWidget, usbContainer);

    // Setup initialisation
    // It's so that if we destroy 0 we do nothing.
    // Each time we select something we destroy "before class"
    // and than we use new class.
    RegulationType=0;
}

MainWindow::~MainWindow()
{
    killTimer(timerId);
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
    // TODO populate the devices and then ( one below )
    printNonRootUSBDevs( ui->listWidget, usbContainer);
}

void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    // TODO connect the device
    // reimplement!!! selection here write below
    // QString string =_usbContainer->writeToDevice(_clickedProduct);
}

void MainWindow::plotChart( double T_set, double actual_time, double T_measured )
{
    PlotTempData.insert(PlotTempData.size(), T_set );
    PlotTime.insert(PlotTime.size(), actual_time );
    PlotTempSet.insert(PlotTempSet.size(), T_measured );

    CurvePlotTempData->setSamples(PlotTime,PlotTempData);
    CurvePlotTempData->setPen( QPen(Qt::red));
    CurvePlotTempSet->setSamples(PlotTime,PlotTempSet);
    CurvePlotTempSet->setPen( QPen(Qt::blue));
    ui->qwtPlot->replot();
}


/// Write data to send - in use use data table → than tempSet
///                    - in use buttons and sliders → then temp
///                    - and so one... "make choice" button shall be added
/// Grab LSB → Grab MSB → Set PID values (double to uint conversion!)
/// and finally program will be ready to test
///buffer[Flag]=1;
/// Calculate temp to set
void MainWindow::timerEvent(QTimerEvent *event)
{
    if((buffer[Flag]==1) && (RegulationType!=0)) {

        double tempToSet = RegulationType->returnTemp(TimmingValue,temp);

        u_int16_t temp_to_device = temp_to_send(tempToSet);
        buffer[TempYoungSet]=temp_to_device&0xFF;
        buffer[TempOldSet]  =(temp_to_device>>8)&0xFF;
        buffer[PID_P]=(u_int8_t)prop;
        buffer[PID_I]=(u_int8_t)integ;
        buffer[PID_D]=(u_int8_t)deriv;

        // TODO this transfer needs to be done via QtUsb
        // // First of all send data
        // libusb_control_transfer(device_handle,LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT,
        //         USB_DATA_IN , 0, 0, buffer, sizeof(buffer), 5000);
        // // Secondly grab data
        // libusb_control_transfer(device_handle,LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
        //         USB_DATA_OUT , 0, 0, buffer, sizeof(buffer), 5000);
        //
        plotChart(tempToSet, timeSecs++, temp_find( buffer[TempYoungADC] | (buffer[TempOldADC]<<8) ) );
    } else {
//        if(USB_Flag_conected==0) ui->textBrowser_usbMessage->setText("no USB, no plot");
//        if(RegulationType==0) ui->textBrowser_usbMessage->setText("No regulation");
    }
}

/// Write data to send - in use use data table → than tempSet
///                    - in use buttons and sliders → then temp
///                    - and so one... "make choice" button shall be added
/// Grab LSB → Grab MSB → Set PID values (double to uint conversion!)
/// and finally program will be ready to test
void MainWindow::on_pushButton_send_clicked()
{

    // TODO in and out transfer
    // than add "start plotting"
    // then maybe send rest of data
    // ADDED for timer start
    // Shall be moved to else part below
    timerId = startTimer(1000);
    if(RegulationType==0) {
//        if(RegulationType==0) ui->textBrowser_usbMessage->setText("No regulation");
    } else {
        buffer[Flag]=1;
        // Calculate temp to set
        // TODO set position
        u_int16_t temp_to_device = temp_to_send(RegulationType->returnTemp(0,temp));
        buffer[TempYoungSet]=temp_to_device&0xFF;
        buffer[TempOldSet]  =(temp_to_device>>8)&0xFF;
        buffer[PID_P]=(u_int8_t)prop;
        buffer[PID_I]=(u_int8_t)integ;
        buffer[PID_D]=(u_int8_t)deriv;

        // TODO this transfer needs to be done via QtUsb
        // // First of all send data
        // libusb_control_transfer(device_handle,LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_OUT,
        //         USB_DATA_IN , 0, 0, buffer, sizeof(buffer), 5000);
        // // Secondly grab data
        // libusb_control_transfer(device_handle,LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE | LIBUSB_ENDPOINT_IN,
        // USB_DATA_OUT , 0, 0, buffer, sizeof(buffer), 5000);
    }

}

void MainWindow::on_radioButton_clicked()
{
    delete RegulationType;
    ui->textBrowser_select->setText("In this setup simple temp curve is send to device to easily solider on halogen lamp-oven. \n"
            "Firstly there is pre heat to cure cheap solder paste, than there is one minute solidering "
            "after that slow dropdown to 100*C");
    RegulationType = new SetupSolidering;
}

void MainWindow::on_radioButton_bistate_clicked()
{
    delete RegulationType;
    ui->textBrowser_select->setText("In this setup controller is operating as bistate controller, temperature is being set in"
            "start tab");
    RegulationType = new SetupBistate;
}

void MainWindow::on_radioButton_tristate_clicked()
{
    delete RegulationType;
    ui->textBrowser_select->setText("In this setup controller is operating as tristate controller, temperature is being set in"
            "start tab");
    RegulationType = new SetupTristate;
}

void MainWindow::on_radioButton_tempCheck_clicked()
{
    delete RegulationType;
    ui->textBrowser_select->setText("In this setup controller is operating as thermometer with constant temperature plot");
    RegulationType = new SetupTempCheck;
}


void MainWindow::on_radioButton_tempCurve_clicked()
{
    delete RegulationType;
    ui->textBrowser_select->setText("In this setup external temperature curve is used - is shall be named dataCurve.txt. \n"
            "Data format shall be: \n\n"
            "\t start\n\t ... \n\tdouble value\n\tdouble value\n\t...\n\tend");
    RegulationType = new SetupExtTempCurv;
}

void MainWindow::on_radioButton_2_clicked()
{
    delete RegulationType;
    ui->textBrowser_select->setText("Not supported yet.");
}

void MainWindow::on_pushButton_loadFile_clicked()
{
    if(RegulationType!=0) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                "/home",
                tr("Text (*.txt *.TXT)"));
        RegulationType->changeFileName(fileName);

    } else {
        ui->textBrowser_select->setText("None type of work selected");
    }
}
