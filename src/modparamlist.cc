#ifndef MODPARAMLIST_H
#include "../include/modparamlist.h"

modparamlist::modparamlist() : parlist(0), param(0)
{
    parlist = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

modparamlist::~modparamlist()
{
    goto_first();
    while(param) {
        delete param;
        goto_next();
    }
    delete parlist;
}

modparam* modparamlist::add_param(modparam* mp)
{
    return (modparam*) parlist->add_at_tail(mp)->get_data();
}

modparam* modparamlist::add_param(
 synthmodnames::SYNTH_MOD_TYPE smt, paramnames::PAR_TYPE pt)
{
    modparam* mp = new modparam(smt, pt);
    if (!parlist->add_at_tail(mp)) {
        delete mp;
        return 0;
    }
    return mp;
}

modparamlist* modparamlist::get_paramlist_for_moduletype(
 synthmodnames::SYNTH_MOD_TYPE smt)
{
    if (smt == synthmodnames::MOD_FIRST)
        return 0;
    modparamlist* mpl = new modparamlist;
    goto_first();
    while(param) {
        if (param->get_moduletype() == smt)
            if (!mpl->add_param(param->get_moduletype(),
             param->get_paramtype())) 
            {
                delete mpl;
                return 0;
            }
        goto_next();
    }
    return mpl;
}

bool modparamlist::validate(
    synthmod* sm, paramnames::PAR_TYPE pt, stockerrs::ERR_TYPE et)
{
    if (!sm) return false;
    if (!stockerrs::check_type(et) && et < stockerrs::ERR_TYPE4)
        return false;
    synthmodnames::SYNTH_MOD_TYPE smt = sm->get_module_type();
    goto_first();
    while (param) {
        if (param->get_moduletype() == smt) {
            if (param->get_paramtype() == pt)
                return param->validate(sm, et);
        }
        goto_next();
    }
    return false;
}

#endif
