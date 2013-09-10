#ifndef DOBJPARAMLIST_H
#define DOBJPARAMLIST_H

#include "dobjparam.h"
#include "linkedlist.h"

#ifndef BARE_MODULES

class dobjparamlist
{
public:
    dobjparamlist();
    ~dobjparamlist();
    dobjparam* add_dobjparam(dobjparam*);
    dobjparam* add_dobjparam(dobjnames::DOBJ_TYPE, dparamnames::DPAR_TYPE);
    dobjparam* goto_first() {
        return (dpar = (dobjparam*)
         (dpar_item = dpar_list->goto_first())->get_data());
    }
    dobjparam* goto_last() {
        return (dpar = (dobjparam*)
         (dpar_item = dpar_list->goto_last())->get_data());
    }
    dobjparam* goto_prev() {
        return (dpar = (dobjparam*)
         (dpar_item = dpar_list->goto_prev())->get_data());
    }
    dobjparam* goto_next() {
        return (dpar = (dobjparam*)
         (dpar_item = dpar_list->goto_next())->get_data());
    }
    dobjparamlist* get_dobjparamlist_for_dobj_type(dobjnames::DOBJ_TYPE);
    bool validate(dobj*, dparamnames::DPAR_TYPE, stockerrs::ERR_TYPE);
private:
    linkedlist* dpar_list;
    ll_item* dpar_item;
    dobjparam* dpar;
};

#endif
#endif
