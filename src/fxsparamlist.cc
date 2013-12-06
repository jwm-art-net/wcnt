#include "../include/fxsparamlist.h"

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
#include <iostream>
void fxsparamlist::dump()
{
    fixstrparam* f = goto_first();
    std::cout << "fxsparamlist::dump()" << std::endl;
    while (f) {
        std::cout << "FXS: '" << param::names::get(f->get_param_type());
        std::cout << "'\t'" << f->get_string_list() << "'" << std::endl;
        f = goto_next();
    }
}
#endif
