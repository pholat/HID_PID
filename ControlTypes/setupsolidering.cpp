#include "setupsolidering.h"

SetupSolidering::SetupSolidering()
{
    _description = 
       "In this setup simple temp curve is send to device to easily solider on halogen lamp-oven. \n"
       "Firstly there is pre heat to cure cheap solder paste, than there is one minute solidering "
       "after that slow dropdown to 100*C";
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

