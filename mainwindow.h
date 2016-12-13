#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QListWidget>
#include <QTime>
#include <QVector>
#include <QtWidgets/QFileDialog>
#include "qwt/qwt_plot_curve.h"
#include "setup.h"
#include "setupbistate.h"
#include "setupexttempcurv.h"
#include "setuptempcheck.h"
#include "setuptristate.h"
#include "setupsolidering.h"

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

    void on_pushButton_link_clicked();

    void on_spinBox_DevNum_valueChanged(int arg1);

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

    void on_pushButton_2_clicked();

    void on_pushButton_loadFile_clicked();

private:
    Ui::MainWindow *ui;
    // ADDED for timer
    int timerId;
    // ADDED for plot
    QwtPlotCurve *CurvePlotTempData;
    QwtPlotCurve *CurvePlotTempSet;
    // ADDED for usable dev count
    int *usableDevCount;
};

#endif // MAINWINDOW_H
