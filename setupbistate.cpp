#include "setupbistate.h"

SetupBistate::SetupBistate()
{

}

SetupBistate::~SetupBistate()
{

}

double SetupBistate::returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/)
{
    return tempValue;
}
