#pragma once

#include <QtCore>
#include <QThread>

void commRun();
// TODO move comms really here with nice signal/slot
class ComWorker : public QObject
{
    Q_OBJECT;

    public: 
        ComWorker();
        ~ComWorker();

    private slots:
        void onTimeout();

    private:
        const double timebase;
        bool usbConStatus;
        QTimer timer;
        friend void commRun();
        bool usbConnected();
};

