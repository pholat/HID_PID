#ifndef SETUPTRISTATE_H
#define SETUPTRISTATE_H

#include "setup.h"

class SetupTristate : public Setup
{
public:
    SetupTristate();
    ~SetupTristate();
    virtual double returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/);
    virtual void changeFileName(QString) {};
};

#endif // SETUPTRISTATE_H
