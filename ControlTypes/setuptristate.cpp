#include "setuptristate.h"

SetupTristate::SetupTristate()
{
    _description = 
        "In this setup controller is operating as tristate controller, temperature is being set in"
        "start tab";
}

SetupTristate::~SetupTristate()
{

}

double SetupTristate::returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/)
{
    return tempValue;
}
