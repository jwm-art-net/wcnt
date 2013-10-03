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
    if (!data)
        return false;
    switch(param::names::category(partype))
    {
    case iocat::DOUBLE:
        return check_value(*(const double*)data, et);
    case iocat::WCINT_T:
        return check_value(*(const wcint_t*)data, et);
    case iocat::SAMP_T:
        return check_value(*(const samp_t*)data, et);
    default:
        return false;
    }
}

