#ifndef DOBJPARAM_H
#include "../include/dobjparam.h"

dobjparam::dobjparam(dobjnames::DOBJ_TYPE dt, paramnames::PAR_TYPE pt):
 dobjtype(dt), partype(pt)
{
}

bool dobjparam::validate(dobj* d, stockerrs::ERR_TYPE et)
{
    void const* data = d->get_param(partype);
    if (!data) return false;
    switch(dobj::get_paramnames()->get_category(partype))
    {
        case CAT_DOUBLE:
            return check_value(*(double const*)data, et);
        case CAT_SHORT:
            return check_value(*(short const*)data, et);
        case CAT_ULONG:
            return check_value(*(unsigned long const*)data, et);
        default:
            return false;
    }
}

#endif
