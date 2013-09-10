#ifndef MODPARAM_H
#include "../include/modparam.h"
#include "../include/jwm_globals.h"
#include "../include/synthmod.h"
#include "../include/checkvalue.h"

modparam::modparam(
 synthmodnames::SYNTH_MOD_TYPE smt, paramnames::PAR_TYPE pt) :
 synthmodule_type(smt), param_type(pt)
{
}

iocat::IOCAT modparam::get_paramcategory()
{
    return (this)
        ? jwm.get_paramnames()->get_category(param_type)
        : iocat::FIRST;
}

bool modparam::validate(synthmod* sm, stockerrs::ERR_TYPE et)
{
    const void* data = sm->get_param(param_type);
    if (!data) return false;
    switch(jwm.get_paramnames()->get_category(param_type))
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

#endif
