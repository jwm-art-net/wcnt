#ifndef MODOBJLIST_H
#define MODOBJLIST_H

#ifndef BARE_MODULES

#include "dobj.h"
#include "moddobj.h"
#include "linkedlist.h"

class moddobjlist
{
public:
    moddobjlist();
    ~moddobjlist();
    moddobj* 
        add_moddobj(moddobj*);
    moddobj* 
        add_moddobj(synthmodnames::SYNTH_MOD_TYPE, dobjnames::DOBJ_TYPE);
    moddobj* goto_first() {
        return mod_dobj = (moddobj*)
         (moddobj_item = moddobj_list->goto_first())->get_data();
    }
    moddobj* goto_last() {
        return mod_dobj = (moddobj*)
         (moddobj_item = moddobj_list->goto_last())->get_data();
    }
    moddobj* goto_prev() {
        return mod_dobj = (moddobj*)
         (moddobj_item = moddobj_list->goto_prev())->get_data();
    }
    moddobj* goto_next(){
        return mod_dobj = (moddobj*)
         (moddobj_item = moddobj_list->goto_next())->get_data();
    }
    moddobjlist* 
        get_moddobjlist_for_moduletype(synthmodnames::SYNTH_MOD_TYPE);
    moddobjlist* 
        get_moddobjlist_of_dobjtype(dobjnames::DOBJ_TYPE);
private:
    linkedlist* moddobj_list;
    ll_item* moddobj_item;
    moddobj* mod_dobj;
};

#endif
#endif
