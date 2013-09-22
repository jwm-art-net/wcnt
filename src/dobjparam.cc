#include "../include/dobjparam.h"
#include "../include/jwm_globals.h"
#include "../include/checkvalue.h"
#include "../include/dobj.h"

dobjparam::dobjparam(dobjnames::DOBJ_TYPE dt, paramnames::PAR_TYPE pt):
 dobjtype(dt), partype(pt)
{
}

bool dobjparam::validate(dobj* d, stockerrs::ERR_TYPE et)
{
    void const* data = d->get_param(partype);
    if (!data) return false;
    switch(paramnames::get_category(partype))
    {
        case iocat::DOUBLE:
            return check_value(*(double const*)data, et);
        case iocat::SHORT:
            return check_value(*(short const*)data, et);
        case iocat::ULONG:
            return check_value(*(unsigned long const*)data, et);
        default:
            return false;
    }
}

