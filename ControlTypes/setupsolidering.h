#ifndef SETUPSOLIDERING_H
#define SETUPSOLIDERING_H

#include "setup.h"

class SetupSwitch;

class SetupSolidering : public Setup
{
    friend SetupSwitch;
    SetupSolidering();
public:
    ~SetupSolidering();
    virtual double returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/);
    virtual void changeFileName(QString) {};

private:
    double tempToReturn;
};

#endif // SETUPSOLIDERING_H
