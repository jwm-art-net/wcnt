#ifndef MODOBJLIST_H
#include "../include/moddobjlist.h"

#ifndef BARE_MODULES

moddobjlist::moddobjlist() :
 moddobj_list(0), moddobj_item(0), mod_dobj(0)
{
    moddobj_list = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

moddobjlist::~moddobjlist()
{
    // delete mod_dobj items in list,
    // leave deletion of list to the list destructor
    goto_first();
    while (mod_dobj) {
        delete mod_dobj;
        goto_next();
    }
    delete moddobj_list;
}

moddobj* moddobjlist::add_moddobj(moddobj* mo)
{
    return (moddobj*)
     (moddobj_item = moddobj_list->add_at_tail(mo))->get_data();
}

moddobj* moddobjlist::add_moddobj(
 synthmodnames::SYNTH_MOD_TYPE smt, dobjnames::DOBJ_TYPE dt)
{
    moddobj* mo = new moddobj(smt, dt);
    ll_item* ll;
    if (!(ll = moddobj_list->add_at_tail(mo))) {
        delete mo;
        return 0;
    }
    return (mod_dobj = (moddobj*)(moddobj_item = ll)->get_data());
}

moddobjlist* moddobjlist::get_moddobjlist_for_moduletype(
 synthmodnames::SYNTH_MOD_TYPE smt)
{
    if (smt <= synthmodnames::MOD_FIRST || smt >= synthmodnames::MOD_LAST)
        return 0;
    moddobjlist* mdl = new moddobjlist();
    goto_first();
    while(mod_dobj != 0) {
        if (mod_dobj->get_moduletype() == smt) {
            if (!mdl->add_moddobj(smt, mod_dobj->get_dobj_type())) {
                delete mdl;
                return 0;
            }
        }
        goto_next();
    }
    return mdl;
}

moddobjlist* moddobjlist::get_moddobjlist_of_dobjtype(
 dobjnames::DOBJ_TYPE dt)
{
    if (dobj::get_dobjnames()->check_type(dt) == dobjnames::DOBJ_FIRST)
    // check_type returns DOBJ_FIRST if dt not user type-------
        return 0; 
    moddobjlist* mdl = new moddobjlist();
    goto_first();
    while(mod_dobj != 0) {
        if (mod_dobj->get_dobj_type() == dt) {
            if (!mdl->add_moddobj(mod_dobj->get_moduletype(), dt)) {
                delete mdl;
                return 0;
            }
        }
        goto_next();
    }
    return mdl;
}

#endif
#endif
