#ifndef FXSPARAMLIST_H
#define FXSPARAMLIST_H

#include "linkedlist.h"
#include "fixstrparam.h"

class fxsparamlist
{
public:
    fxsparamlist();
    ~fxsparamlist();
    fixstrparam*
        add_param(fixstrparam*);
    fixstrparam*
        add_param(char const* string_list, paramnames::PAR_TYPE);
    fixstrparam* goto_first() {
        return fxspar = (fixstrparam*)
         (fxspar_item = fxsparlist->goto_first())->get_data();
    }
    fixstrparam* goto_last() {
        return fxspar = (fixstrparam*)
         (fxspar_item = fxsparlist->goto_last())->get_data();
    }
    fixstrparam* goto_prev() {
        return fxspar = (fixstrparam*)
         (fxspar_item = fxsparlist->goto_prev())->get_data();
    }
    fixstrparam* goto_next() {
        return fxspar = (fixstrparam*)
         (fxspar_item = fxsparlist->goto_next())->get_data();
    }
    fixstrparam* get_fix_str_param(paramnames::PAR_TYPE);

private:
    linkedlist* fxsparlist;
    ll_item* fxspar_item;
    fixstrparam* fxspar;
};

#endif
