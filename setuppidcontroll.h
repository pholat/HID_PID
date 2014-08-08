#ifndef SETUPPIDCONTROLL_H
#define SETUPPIDCONTROLL_H

#include "setup.h"

class SetupPidControll : public Setup
{
public:
    SetupPidControll();
    ~SetupPidControll();
    virtual double returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/);
    virtual void changeFileName(QString){};
};

#endif // SETUPPIDCONTROLL_H
