#include "../include/dobjparam.h"
#include "../include/jwm_globals.h"
#include "../include/checkvalue.h"
#include "../include/dobj.h"

dobjparam::dobjparam(dataobj::TYPE dt, param::TYPE pt):
 dobjtype(dt), partype(pt)
{
}

bool dobjparam::validate(dobj* d, errors::TYPE et)
{
    const void* data = d->get_param(partype);
    if (!data) return false;
    switch(param::names::category(partype))
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

