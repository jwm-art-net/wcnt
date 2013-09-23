#include "../include/fxsparamlist.h"

fixstrparam*
fxsparamlist::add_param(const char* str_list, paramnames::PAR_TYPE pt)
{
    if (!str_list)
        return 0;

    fixstrparam* fsp = 0;
    if ((fsp = get_fix_str_param(pt)))
        return fsp; // already added
    fsp = new fixstrparam(str_list, pt);
    if (!add_at_tail(fsp)) {
        delete fsp;
        return 0;
    }
    return fsp;
}
