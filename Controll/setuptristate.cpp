#include "setuptristate.h"

SetupTristate::SetupTristate( QMap< CB , std::function<void(QString)> >  con ) : Setup(con)
{
    _description = 
        "In this setup controller is operating as tristate controller, temperature is being set in"
        "start tab";
    _connectors[Setup::CB::DESCRIPTION](_description);
}

SetupTristate::~SetupTristate()
{

}

double SetupTristate::returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/)
{
    return tempValue;
}
