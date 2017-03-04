#include "setuppidcontroll.h"
#include "pid.h"

namespace {
    PID pid = PID(0.1, 100, -100, 0.1, 0.01, 0.5);
}

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
    // double valToRet=0;
    double inc = pid.calculate(0, val);
    return valToRet;
}
