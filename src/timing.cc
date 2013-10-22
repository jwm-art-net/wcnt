#include "../include/timing.h"
#include "../include/globals.h"


timing::timing():
 dobj::base(dobj::SIN_TIME), seconds(0.0)
{
}

timing::timing(double s):
 dobj::base(dobj::SIN_BPM),
 seconds(s)
{
}

void timing::register_ui()
{
    register_param(param::SECONDS);
}

ui::dobjitem_list* timing::get_ui_items()
{
    static ui::dobjitem_list items;
    return &items;
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

