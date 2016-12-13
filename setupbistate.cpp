#include "setupbistate.h"

SetupBistate::SetupBistate()
{

}

SetupBistate::~SetupBistate()
{

}

double SetupBistate::returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/)
{
    position=position;
    position=0; // NOT needed as its constant bistable regulation, this line is to avoit warn :)
    return returnVal;
}
