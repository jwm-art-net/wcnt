#ifndef MODPARAM_H
#include "../include/modparam.h"

#ifndef BARE_MODULES

modparam::modparam(
 synthmodnames::SYNTH_MOD_TYPE smt, paramnames::PAR_TYPE pt) :
 synthmodule_type(smt), param_type(pt)
{
}

bool modparam::validate(synthmod* sm, stockerrs::ERR_TYPE et)
{
    void const* data = sm->get_param(param_type);
    if (!data) return false;
    switch(synthmod::get_paramnames()->get_category(param_type))
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
