#ifndef TIMING_H
#include "../include/timing.h"

timing::timing():
 dobj(dobjnames::SIN_TIME), seconds(0.0)
{
    create_params();
}

timing::timing(double s):
 dobj(dobjnames::SIN_BPM),
 seconds(s)
{
    create_params();
}

bool timing::set_param(paramnames::PAR_TYPE pt, void* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_SECONDS:
        set_time(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* timing::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_SECONDS:
        return &seconds;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE timing::validate()
{
    if (!get_dparlist()->validate(
        this, paramnames::PAR_SECONDS, stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_BAR);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

bool timing::done_params = false;

void timing::create_params()
{
    if (done_params == true) return;
    get_dparlist()->add_dobjparam(
     dobjnames::SIN_TIME, paramnames::PAR_SECONDS);
    done_params = true;
}

#endif
