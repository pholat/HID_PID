#include "setupsolidering.h"

SetupSolidering::SetupSolidering()
{
    tempToReturn=20;
}

SetupSolidering::~SetupSolidering()
{

}

double SetupSolidering::returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/)
{
    // Warning: this tempSet is not the global tempSet
    // Yeah - this temp curve is super easy... :)
    if(position<80) tempToReturn++;
    else if(position<140) tempToReturn=100;
    else if(position<280) tempToReturn++;
    else if(position<340);
    else if(position<440) tempToReturn--;
    if(tempToReturn==0) tempToReturn=0;
    return tempToReturn;
}

