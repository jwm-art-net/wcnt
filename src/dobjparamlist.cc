#ifndef DOBJPARAMLIST_H
#include "../include/dobjparamlist.h"

#ifndef BARE_MODULES

dobjparamlist::dobjparamlist() :
 dpar_list(0), dpar_item(0), dpar(0)
{
    dpar_list = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

dobjparamlist::~dobjparamlist()
{
    goto_first();
    while(dpar) {
        delete dpar;
        dpar_item->set_data(0);
        goto_next();
    }
    delete dpar_list;
}

dobjparam* dobjparamlist::add_dobjparam(dobjparam* dp)
{
    return (dpar = (dobjparam*)
     (dpar_item = dpar_list->add_at_tail(dp))->get_data());
}

dobjparam * dobjparamlist::add_dobjparam(
 dobjnames::DOBJ_TYPE dt, dparamnames::DPAR_TYPE dp)
{
    dobjparam* dobjp = new dobjparam(dt, dp);
    dpar = (dobjparam*)
     (dpar_item = dpar_list->add_at_tail(dobjp))->get_data();
    if (!dpar) {
        delete dobjp;
        return 0;
    }
    return dpar;
}

dobjparamlist * dobjparamlist::get_dobjparamlist_for_dobj_type(
 dobjnames::DOBJ_TYPE dt)
{
    dobjparamlist* dpl = new dobjparamlist;
    goto_first();
    while(dpar) {
        if (dpar->get_dobjtype() == dt) {
            if (!dpl->add_dobjparam(dt, dpar->get_dpartype())){
                delete dpl;
                return 0;
            }
        }
        goto_next();
    }
    return dpl;
}

bool dobjparamlist::validate(
 dobj* dbj, dparamnames::DPAR_TYPE dpt, stockerrs::ERR_TYPE et)
{
    if (!dbj) return false;
    if (!stockerrs::check_type(et) && et < stockerrs::ERR_TYPE4)
        return false;
    dobjnames::DOBJ_TYPE dt = dbj->get_object_type();
    goto_first();
    while (dpar) {
        if (dpar->get_dobjtype() == dt) {
            if (dpar->get_dpartype() == dpt)
                return dpar->validate(dbj, et);
        }
        goto_next();
    }
    return false;
}

#endif
#endif