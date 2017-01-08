#include "setuppidcontroll.h"

SetupPidControll::SetupPidControll( QMap< CB , std::function<void(QString)> >  con ) : Setup(con)
{
    _description = "Not supported yet.";
    _connectors[Setup::CB::DESCRIPTION](_description);
}

SetupPidControll::~SetupPidControll()
{

}

double SetupPidControll::returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/)
{
    double valToRet=0;
    return valToRet;
}
