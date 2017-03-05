#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <libusb-1.0/libusb.h>
#include <qwt/qwt_plot_curve.h>
#include <QVector>

#include "Controll/tempTable.h"

#include "AVR_Code/common.h"

#include <QDebug>
#include <QThread>

#include <Controll/ComPoll.h>

namespace {
    Thread compoller;
    double pwm, temp, prop ,integ,deriv;
    const size_t bufsize =USB_DATA_SIZE;
    uchar buffer[bufsize]= {0};

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
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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

    ui->dial_Temp->setMaximum(250);
    ui->dSpinBox_Temp->setMaximum(250);
    ui->dial_PWM->setMaximum(100);
    ui->dSpinBox_PWM->setMaximum(100);

    ui->qwtPlot->replot();

    // Setup initialisation
    ui->radioButton->setChecked(true);
    on_radioButton_clicked();

    // Hide not needed elements
    ui->groupBox->setHidden( true );
    ui->groupBox_2->setHidden( true );

    // Connect Com worker
    connect( ComWorker::instance(),  SIGNAL(ComWorker::instance().message(QStringLis)) , 
            this, SLOT(this->uiMessages( QStringList messages )) );
    // TODO check if it will work without QT moc magic like that
    connect( this, SIGNAL(pushButton_clicked()), CommWorker::instance(),
            SLOT(CommWorker::instance().commRun()) );

}

MainWindow::~MainWindow()
{
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

void MainWindow::setupType( SetupSwitch::Type t ) {
    RegulationType = SetupSwitch::get(
            t,
            QMap<Setup::CB,std::function<void(QString)>> {
                { Setup::CB::DESCRIPTION, [this](QString str) {this->ui->textBrowser_select->setText(str);} },
                { Setup::CB::LOG, [this](QString str) {this->ui->textBrowserLOG->addItem(str);} },
            }
            );
}

void MainWindow::on_radioButton_clicked()
{
    setupType( SetupSwitch::Type::SetupSolidering );
    ui->groupBox->setHidden(true);
    ui->groupBox_2->setHidden(true);
}

void MainWindow::on_radioButton_bistate_clicked()
{
    setupType( SetupSwitch::Type::SetupBistate );
    ui->groupBox->setHidden(true);
    ui->groupBox_2->setHidden(false);
}

void MainWindow::on_radioButton_tristate_clicked()
{
    setupType( SetupSwitch::Type::SetupTristate );
    ui->groupBox->setHidden(true);
    ui->groupBox_2->setHidden(false);
}

void MainWindow::on_radioButton_tempCheck_clicked()
{
    setupType( SetupSwitch::Type::SetupTempCheck );
    ui->groupBox->setHidden(true);
    ui->groupBox_2->setHidden(false);
}


void MainWindow::on_radioButton_tempCurve_clicked()
{
    setupType( SetupSwitch::Type::SetupExtTempCurv);
    ui->groupBox->setHidden(true);
    ui->groupBox_2->setHidden(false);
}

void MainWindow::on_radioButton_2_clicked()
{
    setupType( SetupSwitch::Type::SetupPidControll );
    ui->groupBox->setHidden(false);
    ui->groupBox_2->setHidden(true);
}

void MainWindow::on_pushButton_loadFile_clicked()
{
    // TODO check why file is alredy open
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

void MainWindow::uiMessages( QStringList messages ) 
{
    for ( auto a : messages ) {
        ui->textBrowserLOG().addItem( a );
    }
}

