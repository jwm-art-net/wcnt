#include "../include/modparam.h"
#include "../include/jwm_globals.h"
#include "../include/synthmod.h"
#include "../include/checkvalue.h"

modparam::modparam(
 synthmodnames::SYNTH_MOD_TYPE smt, paramnames::PAR_TYPE pt) :
 synthmodule_type(smt), param_type(pt)
{
    #ifdef DATA_STATS
    STATS_INC
    #endif
}

modparam::~modparam()
{
    #ifdef DATA_STATS
    STATS_DEC
    #endif
}

#ifdef UNUSED
iocat::IOCAT modparam::get_paramcategory()
{
    return (this)
        ? paramnames::get_category(param_type)
        : iocat::FIRST;
}
#endif

bool modparam::validate(synthmod* sm, stockerrs::ERR_TYPE et)
{
    const void* data = sm->get_param(param_type);
    if (!data)
        return false;
    switch(paramnames::get_category(param_type))
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

#ifdef DATA_STATS
STATS_INIT(modparam)
#endif

