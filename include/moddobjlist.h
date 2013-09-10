#ifndef MODOBJLIST_H
#define MODOBJLIST_H

#include "moddobj.h"

// behaves in similiar fashion to topdobjlist - see topdobjlist.h
// for some background...

class moddobjlist
{
public:
    moddobjlist();
    ~moddobjlist();
    moddobj* 
        add_moddobj(synthmodnames::SYNTH_MOD_TYPE,
            dobjnames::DOBJ_TYPE);
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
/*    moddobjlist*
        get_moddobjlist_for_moduletype(synthmodnames::SYNTH_MOD_TYPE);
*/
    moddobj* get_first_of_type(synthmodnames::SYNTH_MOD_TYPE);
    moddobj* get_next_of_type();

private:
    linkedlist* moddobj_list;
    ll_item* moddobj_item;
    moddobj* mod_dobj;
    ll_item* search_result;
    synthmodnames::SYNTH_MOD_TYPE search_type;
};

#endif
