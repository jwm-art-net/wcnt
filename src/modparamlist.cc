#include "../include/modparamlist.h"
#include "../include/synthmod.h"

modparamlist::modparamlist()
{
}

modparamlist::modparamlist(DESTRUCTION d) :
 linkedlist(MULTIREF_OFF, d)
{
}

modparam* modparamlist::add_param(module::TYPE smt, param::TYPE pt)
{
    modparam* mp = new modparam(smt, pt);
    if (!add_at_tail(mp)) {
        delete mp;
        return 0;
    }
    return mp;
}

bool modparamlist::validate(synthmod* sm, param::TYPE pt, errors::TYPE et)
{
    if (!sm) return false;
    if (!errors::stock::chk(et))
        return false;
    #ifdef DEBUG
    switch(errors::stock::category(et)) {
    case errors::CAT_COULD_NOT:
    case errors::CAT_INVALID:
    case errors::CAT_OTHER:
        std::cout << "Is this an error here???" << std::endl;
    default:
        ;
    }
    #endif
    module::TYPE smt = sm->get_module_type();
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

