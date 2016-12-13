#ifndef SETUPEXTTEMPCURV_H
#define SETUPEXTTEMPCURV_H

#include "setup.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QDebug>

/* If no file is selected it's trying to open file in actual dir called dataC*/

class SetupExtTempCurv : public Setup
{
public:
    SetupExtTempCurv();
    ~SetupExtTempCurv();
    virtual double returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/);
    virtual void changeFileName(QString Name)
    {
        fileName=Name;
    }

private:
    QFile FileToRead;
    QTextStream *inputData;
    int dataReadyFlag;
    QString fileName;
    int fileIsSet;
};

#endif // SETUPEXTTEMPCURV_H
