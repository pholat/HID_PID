#pragma once

#include <QtCore>
#include <QThread>
#include <QString>
#include <QStringList>
#include <libusb-1.0/libusb.h>

extern int hotplug_callback( struct libusb_context *ctx, struct libusb_device *dev, libusb_hotplug_event event, void *user_data);
// Here is managed all usb trafic/controll to the avr device
class ComWorker : public QObject
{
    Q_OBJECT;

    public: 
        ~ComWorker();
        static ComWorker &instance();
        ComWorker(ComWorker const&)         = delete;
        void operator=(ComWorker const&)    = delete;
        friend int hotplug_callback( struct libusb_context *ctx, struct libusb_device *dev, libusb_hotplug_event event, void *user_data);

    private slots:
        void onTimeout();

    public slots:
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

