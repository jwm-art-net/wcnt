#ifndef MODOBJLIST_H
#include "../include/moddobjlist.h"

moddobjlist::moddobjlist() :
 moddobj_list(0), moddobj_item(0), mod_dobj(0)
{
    moddobj_list = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

moddobjlist::~moddobjlist()
{
    // delete moddobj data in list,
    // leave deletion of list items to list destructor
    goto_first();
    while (mod_dobj) {
        delete mod_dobj;
        goto_next();
    }
    delete moddobj_list;
}

moddobj* moddobjlist::add_moddobj(synthmodnames::SYNTH_MOD_TYPE smt,
    dobjnames::DOBJ_TYPE fc)
{
    if (smt <= synthmodnames::FIRST || smt >= synthmodnames::LAST)
        return 0;
    moddobj* mo = new moddobj(smt, fc);
    moddobj_item = moddobj_list->add_at_tail(mo);
    return mod_dobj = (moddobj*)moddobj_item->get_data();
}

/*
moddobjlist* moddobjlist::get_moddobjlist_for_moduletype(
                                synthmodnames::SYNTH_MOD_TYPE smt)
{
    if (smt <= synthmodnames::MOD_FIRST || smt >= synthmodnames::MOD_LAST)
        return 0;
    moddobjlist* mdl = new moddobjlist();
    goto_first();
    while(mod_dobj) {
        if (mod_dobj->get_moduletype() == smt) {
            if (!mdl->add_moddobj(smt, mod_dobj->get_first_child())) {
                delete mdl;
                return 0;
            }
        }
        goto_next();
    }
    return mdl;
}
*/

moddobj* moddobjlist::get_first_of_type(synthmodnames::SYNTH_MOD_TYPE smt)
{
    search_type = smt;
    search_result = 0;
    if (smt <= synthmodnames::FIRST || smt >= synthmodnames::LAST)
        return 0;
    goto_first();
    while (mod_dobj) {
        if (mod_dobj->get_moduletype() == search_type)
            return mod_dobj = (moddobj*)
                (search_result = moddobj_item)->get_data();
        goto_next();
    }
    return 0;
}

moddobj* moddobjlist::get_next_of_type()
{
    if (search_result == 0)
        return 0;
    mod_dobj = (moddobj*)(search_result->get_data());
    goto_next(); // !otherwise it gets stuck..
    while(mod_dobj) {
        if (mod_dobj->get_moduletype() == search_type) {
            return mod_dobj = (moddobj*)
                (search_result = moddobj_item)->get_data();
        }
        goto_next();
    }
    search_result = 0;
    return 0;
}
#endif
