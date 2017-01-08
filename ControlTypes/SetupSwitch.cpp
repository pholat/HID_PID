#include "SetupSwitch.h"

#include "setuptristate.h"
#include "setupbistate.h"
#include "setuptempcheck.h"
#include "setupexttempcurv.h"
#include "setuppidcontroll.h"
#include "setupsolidering.h"

Setup *SetupSwitch::_ret=0;

// TODO Add constructors with our magicall neat beautiful map
Setup *SetupSwitch::get(const Type what, QMap< Setup::CB, std::function<void(QString)> > cbMap ) {
    delete _ret;
    switch (what) {
        case SetupTempCheck:
            _ret = new class SetupTempCheck(cbMap);
            break;
        case SetupTristate:
            _ret = new class SetupTristate(cbMap);
            break;
        case SetupBistate:
            _ret = new class SetupBistate(cbMap);
            break;
        case SetupExtTempCurv:
            _ret = new class SetupExtTempCurv(cbMap);
            break;
        case SetupPidControll:
            _ret = new class SetupPidControll(cbMap);
            break;
        case SetupSolidering:
            _ret = new class SetupSolidering(cbMap);
            break;
        default: _ret = 0;
    }

    return _ret;
}
