#ifndef SETUP_H
#define SETUP_H

/* This class shall be purely virtual - it's not, as I do not know why Qt will not let me.
 * There should be added virtual ControllPas function which would pass values which are
 * set in controll box , global values shall be in another class or shall be in maiwindow.h
 * */

#include <QString>

class Setup
{
public:
    enum {SimpleCurve,BistateReg,TristateReg,TempCheck,ExternCurve} SetType;
    //Setup();
    virtual ~Setup();
    virtual double returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/) =0;
    virtual void changeFileName(QString) {};
};

#endif // SETUP_H
