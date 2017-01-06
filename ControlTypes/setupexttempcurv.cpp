#include "setupexttempcurv.h"

SetupExtTempCurv::SetupExtTempCurv()
{
    _description =
        "In this setup external temperature curve is used - is shall be named dataCurve.txt. \n"
        "Data format shall be: \n\n"
        "\t ... \n\tdouble value\n\tdouble value\n\t...";

}

SetupExtTempCurv::~SetupExtTempCurv()
{
}

void SetupExtTempCurv::processFile() {
    qDebug() << "FILE LOADED!";
    QFile fileToRead;
    qDebug() << "About to load: " << fileName;
    fileToRead.setFileName(fileName);
    fileToRead.open(QIODevice::ReadOnly);
    bool dataReadyFlag = fileToRead.open(QIODevice::ReadOnly);
//    if(!dataReadyFlag) {
//        qDebug() << "File read failure!"; //Here shall be kind of error
//        qDebug() << fileToRead.errorString();
//    } else {
        QTextStream fread(&fileToRead);
        for ( QString line=""; !(fread.atEnd()); line=fread.readLine() ) {
            for ( auto a: line.split(",") ) {
                _values.push_back( a.toDouble() );
            }
        }
//    }
    fileToRead.close();
}


double SetupExtTempCurv::returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/)
{
    if ( ! ( position < _values.size() ) ) { qDebug() << "Nothing to return"; return 0; }
    qDebug() << "Position: " << position << " value: " << _values[position];
    return _values[position];
}
