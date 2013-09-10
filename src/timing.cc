#ifndef TIMING_H
#include "../include/timing.h"

timing::timing():
 dobj(dobjnames::SIN_TIME), seconds(0.0)
{
#ifndef BARE_MODULES
    create_dparams();
#endif
}

timing::timing(double s):
 dobj(dobjnames::SIN_BPM),
 seconds(s)
{
#ifndef BARE_MODULES
    create_dparams();
#endif
}

#ifndef BARE_MODULES

bool timing::set_dparam(dparamnames::DPAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case dparamnames::DPAR_SECONDS:
        set_time(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void* timing::get_dparam(dparamnames::DPAR_TYPE pt)
{
    switch(pt)
    {
    case dparamnames::DPAR_SECONDS:
        return &seconds;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE timing::validate()
{
    if (!get_dparlist()->validate(
        this, dparamnames::DPAR_SECONDS, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_dparnames()->get_name(dparamnames::DPAR_BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

bool timing::done_dparams = false;

void timing::create_dparams()
{
    if (done_dparams == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_TIME, dparamnames::DPAR_SECONDS);
    done_dparams = true;
}

#endif // BARE_MODULES
#endif
