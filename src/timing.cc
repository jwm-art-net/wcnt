#include "../include/timing.h"
#include "../include/jwm_globals.h"
#include "../include/dobjparamlist.h"


timing::timing():
 dobj(dataobj::SIN_TIME), seconds(0.0)
{
    init_first();
}

timing::timing(double s):
 dobj(dataobj::SIN_BPM),
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

errors::TYPE timing::validate()
{
    if (!validate_param(param::SECONDS, errors::NEGATIVE))
        return errors::NEGATIVE;

    return errors::NO_ERROR;
}



void timing::init_first()
{
    if (done_first()) return;
    register_param(param::SECONDS);
}

