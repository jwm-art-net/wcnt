#ifndef FXSPARAMLIST_H
#include "../include/fxsparamlist.h"

fxsparamlist::fxsparamlist() : fxsparlist(0), fxspar(0)
{
    fxsparlist = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

fxsparamlist::~fxsparamlist()
{
    goto_first();
    while(fxspar) {
        delete fxspar;
        goto_next();
    }
    delete fxsparlist;
}

fixstrparam* fxsparamlist::add_param(fixstrparam* fsp)
{
    if (get_fix_str_param(fsp->get_param_type()))
        return 0; // already added
    return (fixstrparam*)fxsparlist->add_at_tail(fsp)->get_data();
}

fixstrparam* fxsparamlist::
add_param(char const* str_list, paramnames::PAR_TYPE pt)
{
    if (str_list == 0)
        return 0;
    if (get_fix_str_param(pt))
        return 0; // already added
    fixstrparam* fsp = new fixstrparam(str_list, pt);
    if (!fxsparlist->add_at_tail(fsp)) {
        delete fsp;
        return 0;
    }
    return fsp;
}

fixstrparam* fxsparamlist::get_fix_str_param(paramnames::PAR_TYPE pt)
{
    goto_first();
    while(fxspar) {
        if (fxspar->get_param_type() == pt)
            return fxspar;
        goto_next();
    }
    return 0;
}

#endif
