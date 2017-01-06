#include "SetupSwitch.h"

#include "setuptristate.h"
#include "setupbistate.h"
#include "setuptempcheck.h"
#include "setupexttempcurv.h"
#include "setuppidcontroll.h"
#include "setupsolidering.h"

Setup *SetupSwitch::_ret=0;

Setup *SetupSwitch::get(const Type what) {
    delete _ret;
    switch (what) {
        case SetupTempCheck:
            _ret = new class SetupTempCheck;
            break;
        case SetupTristate:
            _ret = new class SetupTristate;
            break;
        case SetupBistate:
            _ret = new class SetupBistate;
            break;
        case SetupExtTempCurv:
            _ret = new class SetupExtTempCurv;
            break;
        case SetupPidControll:
            _ret = new class SetupPidControll;
            break;
        case SetupSolidering:
            _ret = new class SetupSolidering;
            break;
        default: _ret = 0;
    }
    return _ret;
}
