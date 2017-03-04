#include <QtCore>
#include <QThread>

#include <QDebug>
#include <QTimer>
#include "ComPoll.h"

void ComWorker::onTimeout() {
	// get propper values from controllers - timescalled by n*(1/timebase_)
	// set controler controll data on/off
	// send/receive data
}

void Thread::run()
{
	qDebug()<<"From work thread: "<<currentThreadId();
	QTimer timer;
	connect(&timer, SIGNAL(timeout()), &wixxx , SLOT(onTimeout()));
	timer.start(_timebase);
	exec();
}
