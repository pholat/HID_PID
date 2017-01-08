#ifndef SETUPEXTTEMPCURV_H
#define SETUPEXTTEMPCURV_H

#include "setup.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QDebug>

#include <QVector>

/* If no file is selected it's trying to open file in actual dir called dataC*/
class SetupSwitch;

class SetupExtTempCurv : public Setup
{
    friend SetupSwitch;
    SetupExtTempCurv( QMap< CB , std::function<void(QString)> >  con );
public:
    ~SetupExtTempCurv();
    virtual double returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/);
    void processFile();

private:
    QVector< double> _values;
};

#endif // SETUPEXTTEMPCURV_H
