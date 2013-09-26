#include "../include/modparamlist.h"
#include "../include/synthmod.h"

modparamlist::modparamlist()
{
}

modparamlist::modparamlist(DESTRUCTION d) :
 linkedlist(MULTIREF_OFF, d)
{
}

modparam* modparamlist::add_param(
    synthmodnames::SYNTHMOD_TYPE smt, paramnames::PAR_TYPE pt)
{
    modparam* mp = new modparam(smt, pt);
    if (!add_at_tail(mp)) {
        delete mp;
        return 0;
    }
    return mp;
}

bool modparamlist::validate(
    synthmod* sm, paramnames::PAR_TYPE pt, stockerrs::ERR_TYPE et)
{
    if (!sm) return false;
    if (!stockerrs::check_type(et) && et < stockerrs::ERR_TYPE4)
        return false;
    synthmodnames::SYNTHMOD_TYPE smt = sm->get_module_type();
    modparam* param = goto_first();
    while (param) {
        if (param->get_moduletype() == smt) {
            if (param->get_paramtype() == pt)
                return param->validate(sm, et);
        }
        param = goto_next();
    }
    return false;
}

