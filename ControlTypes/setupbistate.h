#ifndef SETUPBISTATE_H
#define SETUPBISTATE_H

#include "setup.h"

class SetupSwitch;

class SetupBistate : public Setup
{
    friend SetupSwitch;
    SetupBistate();
public:
    ~SetupBistate();
    virtual double returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/);
    virtual void changeFileName(QString) {};
};

#endif // SETUPBISTATE_H
