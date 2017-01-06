#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <libusb-1.0/libusb.h>
#include <qwt/qwt_plot_curve.h>
#include <QVector>

#include "tempTable.h"

#include "QtUsb/usb-container.h"

#include <QDebug>
#include "ControlTypes/SetupSwitch.h"

// TODO move to common include...
// libUSB members - these tells us on what endpoints we work
// this actually might have been done in one transmitt
#define USB_DATA_OUT 		2	//Device to PC
#define USB_DATA_IN 		4	//Fine PC to Device

namespace {

    double pwm, temp, prop ,integ,deriv;
    
    const size_t bufsize =8;
    uchar buffer[bufsize]= {0};
    enum bufferByte {
        Flag,TempYoungADC,TempOldADC,TempYoungSet,TempOldSet,PID_P,PID_I,PID_D
    };

    UsbDev *usbDev;
    
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
    
    void printNonRootUSBDevs( QListWidget *listWidget, UsbContainer &usbContainer) {
        listWidget->clear();
        listWidget->addItems(usbContainer.listNonRootDevices());
    }
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
    ui->dial_PWM->setMaximum(100);
    ui->dSpinBox_PWM->setMaximum(100);

    ui->qwtPlot->replot();

    // USB initialised
    ui->textBrowserLOG->addItems(usbErrorLog.split("\n"));
    printNonRootUSBDevs( ui->listWidget, *usbcontainer);

    // Setup initialisation
    // It's so that if we destroy 0 we do nothing.
    // Each time we select something we destroy "before class"
    // and than we use new class.
    // By default we setup halogen lamp soldering curve - btw. RegulationType cries for factory
    ui->radioButton->setChecked(true);
    on_radioButton_clicked();

    // Hide not needed elements
    ui->groupBox->setHidden( true );
    ui->groupBox_2->setHidden( true );
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
    printNonRootUSBDevs( ui->listWidget, *usbcontainer);
}

/// Device from list selected
void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    usbDev = usbcontainer->getDevice( index.row() );
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
/// buffer[Flag]=1;
/// Calculate temp to set
void MainWindow::timerEvent(QTimerEvent *event)
{
    if( (usbDev!=0) && (buffer[Flag]==1) && (RegulationType!=0)) {

        double tempToSet = RegulationType->returnTemp(TimmingValue++,temp);

        u_int16_t temp_to_device = temp_to_send(tempToSet);
        buffer[TempYoungSet]=temp_to_device&0xFF;
        buffer[TempOldSet]  =(temp_to_device>>8)&0xFF;
        buffer[PID_P]=(u_int8_t)prop;
        buffer[PID_I]=(u_int8_t)integ;
        buffer[PID_D]=(u_int8_t)deriv;

        usbDev->control_transfer(Endpoint::Direction::Out, USB_DATA_IN, buffer, bufsize );
        usbDev->control_transfer(Endpoint::Direction::In, USB_DATA_OUT, buffer, bufsize );
        plotChart(tempToSet, timeSecs++, temp_find( buffer[TempYoungADC] | (buffer[TempOldADC]<<8) ) );

        qDebug() << "ADC  val: " << ( buffer[TempYoungADC] | (buffer[TempOldADC]<<8) );
        qDebug() << "TEMP val: " << temp_find( buffer[TempYoungADC] | (buffer[TempOldADC]<<8) );
    } else {
        if( usbDev == 0 ) ui->textBrowserLOG->addItem("no USB, no plot");
        if(RegulationType==0) ui->textBrowserLOG->addItem("No regulation");
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
    TimmingValue = 0;
    timerId = startTimer(1000);
    if( (usbDev==0) || RegulationType==0 ) {
        if( usbDev == 0 ) ui->textBrowserLOG->addItem("no USB, no plot");
        if(RegulationType==0) ui->textBrowserLOG->addItem("No regulation");
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

        usbDev->control_transfer(Endpoint::Direction::Out, USB_DATA_IN, buffer, bufsize );
        usbDev->control_transfer(Endpoint::Direction::In, USB_DATA_OUT, buffer, bufsize );
    }

}

void MainWindow::on_radioButton_clicked()
{
    RegulationType = SetupSwitch::get(SetupSwitch::Type::SetupSolidering);
    ui->textBrowser_select->setText( RegulationType->getDescription() );
    ui->groupBox->setHidden(true);
    ui->groupBox_2->setHidden(true);
}

void MainWindow::on_radioButton_bistate_clicked()
{
    RegulationType = SetupSwitch::get(SetupSwitch::Type::SetupBistate);
    ui->textBrowser_select->setText( RegulationType->getDescription() );
    ui->groupBox->setHidden(true);
    ui->groupBox_2->setHidden(false);
}

void MainWindow::on_radioButton_tristate_clicked()
{
    RegulationType = SetupSwitch::get(SetupSwitch::Type::SetupTristate);
    ui->textBrowser_select->setText( RegulationType->getDescription() );
    ui->groupBox->setHidden(true);
    ui->groupBox_2->setHidden(false);
}

void MainWindow::on_radioButton_tempCheck_clicked()
{
    RegulationType = SetupSwitch::get(SetupSwitch::Type::SetupTempCheck);
    ui->textBrowser_select->setText( RegulationType->getDescription() );
    ui->groupBox->setHidden(true);
    ui->groupBox_2->setHidden(false);
}


void MainWindow::on_radioButton_tempCurve_clicked()
{
    RegulationType = SetupSwitch::get(SetupSwitch::Type::SetupExtTempCurv);
    ui->textBrowser_select->setText( RegulationType->getDescription() );
    ui->groupBox->setHidden(true);
    ui->groupBox_2->setHidden(false);
}

void MainWindow::on_radioButton_2_clicked()
{
    RegulationType = SetupSwitch::get(SetupSwitch::Type::SetupPidControll);
    ui->textBrowser_select->setText( RegulationType->getDescription() );
    ui->groupBox->setHidden(false);
    ui->groupBox_2->setHidden(true);
}

void MainWindow::on_pushButton_loadFile_clicked()
{
    if(RegulationType!=0) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                "/home",
                tr("Text (*.txt *.TXT)"));
        RegulationType->changeFileName(fileName);
        RegulationType->processFile();
    } else {
        ui->textBrowser_select->setText("None type of work selected");
    }
}
