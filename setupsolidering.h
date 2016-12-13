#ifndef SETUPSOLIDERING_H
#define SETUPSOLIDERING_H

#include "setup.h"

class SetupSolidering : public Setup
{
public:
    SetupSolidering();
    ~SetupSolidering();
    virtual double returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/);
    virtual void changeFileName(QString) {};

private:
    double tempToReturn;
};

#endif // SETUPSOLIDERING_H
