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

    private slots:
        void onTimeout();
        const double timebase;
        QTimer timer;
        friend void commRun();
};

