#ifndef SETUPBISTATE_H
#define SETUPBISTATE_H

#include "setup.h"

class SetupBistate : public Setup
{
public:
    SetupBistate();
    ~SetupBistate();
    virtual double returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/);
    virtual void changeFileName(QString) {};
};

#endif // SETUPBISTATE_H
