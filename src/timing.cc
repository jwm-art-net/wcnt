#include "../include/timing.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"

#ifdef DEBUG_MSG
#include <cstdio>
#endif


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
    switch(pt)
    {
        case paramnames::SECONDS:
            set_time(*(double*)data);
            return true;
        default:
            return false;
    }
}

void const* timing::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::SECONDS:
            return &seconds;
        default:
            return 0;
    }
}

stockerrs::ERR_TYPE timing::validate()
{
    if (!jwm.get_dparlist()->validate(
        this, paramnames::SECONDS, stockerrs::ERR_NEGATIVE))
    {
        dobjerr("%s", jwm.get_paramnames()->get_name(paramnames::BAR));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

bool timing::done_params = false;

void timing::create_params()
{
    if (done_params == true) return;
    jwm.get_dparlist()->add_dobjparam(
        dobjnames::SIN_TIME, paramnames::SECONDS);
    done_params = true;
}

