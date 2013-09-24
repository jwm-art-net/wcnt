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
    const void* data = d->get_param(partype);
    if (!data)
        return false;
    switch(paramnames::get_category(partype))
    {
        case iocat::DOUBLE:
            return check_value(*(const double*)data, et);
        case iocat::SHORT:
            return check_value(*(const short*)data, et);
        case iocat::SAMP_T:
            return check_value(*(const samp_t*)data, et);
        default:
            return false;
    }
}

