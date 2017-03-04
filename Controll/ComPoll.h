#pragma once

#include <QtCore>
#include <QThread>

// TODO check how exec/etc works - we want timer overflow frequent, not once
// TODO move actuall Com code here
class ComWorker : public QObject
{
    Q_OBJECT;

    private slots:
	void onTimeout();
};


class Thread : public QThread 
{
	Q_OBJECT;

	private:
	double timebase_;
	void run();

	public:
	Thread( double timebase );
};
