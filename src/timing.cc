#include "../include/timing.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"


timing::timing():
 dobj(dobjnames::SIN_TIME), seconds(0.0)
{
    init_first();
}

timing::timing(double s):
 dobj(dobjnames::SIN_BPM),
 seconds(s)
{
    init_first();
}

bool timing::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::SECONDS:
            set_time(*(double*)data);
            return true;
        default:
            return false;
    }
}

const void* timing::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::SECONDS:
            return &seconds;
        default:
            return 0;
    }
}

stockerrs::ERR_TYPE timing::validate()
{
    if (!jwm.get_dparlist()->validate(
        this, param::SECONDS, stockerrs::ERR_NEGATIVE))
    {
        dobjerr("%s", param::names::get(param::BAR));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}



void timing::init_first()
{
    if (done_first()) return;
    register_param(param::SECONDS);
}

