#ifndef DOBJPARAM_H
#include "../include/dobjparam.h"

#ifndef BARE_MODULES
dobjparam::dobjparam(dobjnames::DOBJ_TYPE dt, dparamnames::DPAR_TYPE dpt):
 dobjtype(dt), dpartype(dpt)
{
}

bool dobjparam::validate(dobj* d, stockerrs::ERR_TYPE et)
{
    void const* data = d->get_dparam(dpartype);
    if (!data) return false;
    switch(dobj::get_dparnames()->get_category(dpartype))
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
#endif
