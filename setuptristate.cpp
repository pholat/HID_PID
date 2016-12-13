#include "setuptristate.h"

SetupTristate::SetupTristate()
{
}

SetupTristate::~SetupTristate()
{

}

double SetupTristate::returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/)
{
    return tempValue;
}
