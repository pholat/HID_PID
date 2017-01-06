#ifndef SETUPTEMPCHECK_H
#define SETUPTEMPCHECK_H

#include "setup.h"

class SetupSwitch;

class SetupTempCheck : public Setup
{
    friend SetupSwitch;
    SetupTempCheck();
public:
    ~SetupTempCheck();
    virtual double returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/);
    virtual void changeFileName(QString) {};
};

#endif // SETUPTEMPCHECK_H
