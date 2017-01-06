#include "setupexttempcurv.h"

SetupExtTempCurv::SetupExtTempCurv()
{
    _description = 
        "In this setup external temperature curve is used - is shall be named dataCurve.txt. \n"
        "Data format shall be: \n\n"
        "\t start\n\t ... \n\tdouble value\n\tdouble value\n\t...\n\tend";

    fileName="";
    QFile fileToRead;
    fileToRead.setFileName("dataCurve.txt");
    fileToRead.open(QIODevice::ReadOnly);
    bool dataReadyFlag = fileToRead.open(QIODevice::ReadWrite);
    if(!dataReadyFlag); //Here shall be kind of error
    else { 
        QTextStream fread(&fileToRead);
        for ( QString line=""; !(fread.atEnd()); line=fread.readLine() ) {
            for ( auto a: line.split(",") ) {
                _values.push_back( a.toDouble() );
            }
        }
    }
}

SetupExtTempCurv::~SetupExtTempCurv()
{
}

double SetupExtTempCurv::returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/)
{
    return _values[position];

}
