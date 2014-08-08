#include "setupexttempcurv.h"

SetupExtTempCurv::SetupExtTempCurv()
{
    fileIsSet=0;
    fileName="0";
    FileToRead.setFileName("dataCurve.txt");
    dataReadyFlag = FileToRead.open(QIODevice::ReadWrite);
    if(!dataReadyFlag); //Here shall be kind of error
    else inputData = new QTextStream(&FileToRead);
}

SetupExtTempCurv::~SetupExtTempCurv()
{
    delete inputData;
}

double SetupExtTempCurv::returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/)
{
    if(!fileName.compare("0"));
    else if (fileIsSet==0){
        delete inputData;
        FileToRead.reset();
        FileToRead.setFileName(fileName);
        dataReadyFlag = FileToRead.open(QIODevice::ReadWrite);
        if(!dataReadyFlag); //Here shall be kind of error
        else inputData = new QTextStream(&FileToRead);
        fileIsSet=1;
    }

    QString line;
    double returnVal=0;
    if(!dataReadyFlag)
    {
        returnVal=100;
    }else
    {
        if(!inputData->atEnd())
        {
            line = inputData->readLine(10);
            qDebug() <<"Newline is \n" << line;
        } else return 0;

        if(!line.compare("end"))
        {
            returnVal=0;
            qDebug()<<"End";
        }
        else if(!line.compare("start"))
        {
            returnVal=0;
            qDebug()<<"Start\n";
        }
        else
        {
            qDebug()<<"Conversion: ";
            returnVal=line.toDouble();
            qDebug()<<returnVal<<"\n";
        }
    }
    return returnVal;
}
