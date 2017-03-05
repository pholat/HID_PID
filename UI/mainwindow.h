#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QListWidget>
#include <QTime>
#include <QVector>
#include <QStringList>
#include <QtWidgets/QFileDialog>
#include "qwt/qwt_plot_curve.h"
#include "Controll/setup.h"
#include "Controll/setupbistate.h"
#include "Controll/setupexttempcurv.h"
#include "Controll/setuptempcheck.h"
#include "Controll/setuptristate.h"
#include "Controll/setupsolidering.h"

#include <memory> // unique poitner
#include "Controll/SetupSwitch.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Setup *RegulationType;
    int TimmingValue;

public slots:
    void listview_populate_usb_devices();

private slots:
    void on_dial_PWM_sliderMoved(int position);
    void on_dial_Temp_sliderMoved(int position);
    void on_verticalSlider_P_sliderMoved(int position);
    void on_verticalSlider_I_sliderMoved(int position);
    void on_verticalSlider_D_sliderMoved(int position);
    void on_dSpinBox_P_valueChanged(double arg1);
    void on_dSpinBox_I_valueChanged(double arg1);
    void on_dSpinBox_D_valueChanged(double arg1);
    void on_verticalSlider_P_valueChanged(int value);
    void on_verticalSlider_I_valueChanged(int value);
    void on_verticalSlider_D_valueChanged(int value);

    void on_listWidget_clicked(const QModelIndex &index);
    //ADDED for timer
    void timerEvent(QTimerEvent *event);
    void on_pushButton_send_clicked();
    void on_radioButton_clicked();
    void on_radioButton_bistate_clicked();
    void on_radioButton_tristate_clicked();
    void on_radioButton_tempCheck_clicked();
    void on_radioButton_tempCurve_clicked();
    void on_radioButton_2_clicked();
    void on_pushButton_loadFile_clicked();
    // Added for callbacks
    void uiMessages( QStringList messages );

private:
    Ui::MainWindow *ui;
    // ADDED for timer
    int timerId;
    // ADDED for plot
    QwtPlotCurve *CurvePlotTempData;
    QwtPlotCurve *CurvePlotTempSet;
    void plotChart( double T_set, double actual_time, double T_measured );
    void setupType( SetupSwitch::Type t );
};

#endif // MAINWINDOW_H
