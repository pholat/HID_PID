#ifndef SETUPSWITCH_H
#define SETUPSWITCH_H

#include "setup.h"

/// Class switching through usage types. Types are all private and only switch has access
/// TODO change pointer type to new fancy one
class SetupSwitch
{
private:
    static Setup *_ret;
public:
    enum Type{
        SetupTempCheck,
        SetupTristate,
        SetupBistate,
        SetupExtTempCurv,
        SetupPidControll,
        SetupSolidering,
    };
    SetupSwitch();
    virtual ~SetupSwitch();
    static Setup *get(const Type what);
};

#endif /* SETUPSWITCH_H */
