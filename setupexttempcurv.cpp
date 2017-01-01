#include "setupexttempcurv.h"

SetupExtTempCurv::SetupExtTempCurv()
{
    fileName="";
    QFile fileToRead = QFile("dataCurve.txt");
    bool dataReadyFlag = fileToRead.open(QIODevice::ReadWrite);
    if(!dataReadyFlag); //Here shall be kind of error
    else { 
        QTextStream fread(&fileToRead);
        for ( QString line=""; !(fread.atEnd()); line=fread.readLine() ) {
            for ( auto a: line.split(",") {
                _values.push_back( double(a) );
            }
        }
    }
}

SetupExtTempCurv::~SetupExtTempCurv()
{
}

double SetupExtTempCurv::returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/)
{
    return _values.at( position );
}
