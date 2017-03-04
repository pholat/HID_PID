#include "setuptempcheck.h"

SetupTempCheck::SetupTempCheck( QMap< CB , std::function<void(QString)> >  con ) : Setup(con)
{
    _description = 
        "In this setup controller is operating as thermometer with constant temperature plot";
    _connectors[Setup::CB::DESCRIPTION](_description);
}

SetupTempCheck::~SetupTempCheck()
{

}

double SetupTempCheck::returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/)
{
    position=position;
    position=0; // NOT needed as its constant bistable regulation, this line is to avoid warn :)
    double returnVal=tempValue;
    return returnVal=0; // Well is only temp check so it's ok to draw horizontal line
}
