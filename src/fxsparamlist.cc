#include "../include/fxsparamlist.h"
#include "../include/debug.h"

fixstrparam*
fxsparamlist::add_param(const char* str_list, param::TYPE pt)
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

#ifdef DEBUG
void fxsparamlist::dump()
{
    fixstrparam* f = goto_first();
    while (f) {
        D_BUG("FXS: '%s'\t'%s'\n" , param::names::get(f->get_param_type()),
                                    f->get_string_list());
        f = goto_next();
    }
}
#endif
