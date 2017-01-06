#include "setupbistate.h"

SetupBistate::SetupBistate()
{
    _description = 
        "In this setup controller is operating as bistate controller, temperature is being set in"
        "start tab";

}

SetupBistate::~SetupBistate()
{

}

double SetupBistate::returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/)
{
    return tempValue;
}
