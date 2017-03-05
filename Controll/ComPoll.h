#pragma once

#include <QtCore>
#include <QThread>
#include <QString>
#include <QStringList>

// Here is managed all usb trafic/controll to the avr device
class ComWorker : public QObject
{
    Q_OBJECT;

    public: 
        ~ComWorker();
        static ComWorker &instance();
        ComWorker(ComWorker const&)         = delete;
        void operator=(ComWorker const&)    = delete;

    private slots:
        void onTimeout();
        void commRun();

    signals:
        void message(QStringList message);

    private:
        ComWorker();
        const double timebase;
        bool usbConStatus;
        QTimer timer;
        bool usbConnected();
};

