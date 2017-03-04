#include <QtCore>
#include <QThread>

#include <QDebug>
#include <QTimer>
#include "ComPoll.h"

static ComWorker comWorker;

ComWorker::ComWorker()
{
}

void ComWorker::onTimeout() {
	// get propper values from controllers - timescalled by n*(1/timebase_)
	// set controler controll data on/off
	// send/receive data
}

void commRun()
{
	connect(&comWorker.timer, SIGNAL(timeout()), &comWorker , SLOT(onTimeout()));
	comWorker.timer.start(comWorker.timebase);
}
