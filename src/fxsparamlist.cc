#ifndef FXSPARAMLIST_H
#include "../include/fxsparamlist.h"

fixstrparam*
fxsparamlist::add_param(char const* str_list, paramnames::PAR_TYPE pt)
{
    if (!str_list)
        return 0;
    if (get_fix_str_param(pt))
        return 0; // already added
    fixstrparam* fsp = new fixstrparam(str_list, pt);
    if (!add_at_tail(fsp)) {
        delete fsp;
        return 0;
    }
    return fsp;
}

#endif
