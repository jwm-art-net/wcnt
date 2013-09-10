#ifndef DOBJDOBJLIST_H
#define DOBJDOBJLIST_H

#ifndef BARE_MODULES

#include "dobj.h"
#include "dobjdobj.h"
#include "linkedlist.h"

// good name for a class I think, some may find it ugly, but beauty is
// in the eye of the beholder. that's what I keep telling myself anyway.

class dobjdobjlist
{
public:
    dobjdobjlist();
    ~dobjdobjlist();
    dobjdobj* add_dobjdobj(dobjdobj*);
    dobjdobj* add_dobjdobj(dobjnames::DOBJ_TYPE, dobjnames::DOBJ_TYPE);
    dobjdobj* goto_first() { 
        return dobj_dobj = (dobjdobj*)
         (dobjdobj_item = dobjdobj_list->goto_first())->get_data();
    }
    dobjdobj* goto_last() {
        return dobj_dobj = (dobjdobj*)
         (dobjdobj_item = dobjdobj_list->goto_last())->get_data();
    }
    dobjdobj* goto_prev() {
        return dobj_dobj = (dobjdobj*)
         (dobjdobj_item = dobjdobj_list->goto_prev())->get_data();
    }
    dobjdobj* goto_next() {
        return dobj_dobj = (dobjdobj*)
         (dobjdobj_item = dobjdobj_list->goto_next())->get_data();
    }
    dobjdobjlist* get_dobjdobjlist_for_dobjtype(dobjnames::DOBJ_TYPE);
    dobjdobjlist* get_dobjdobjlist_of_sprogs(dobjnames::DOBJ_TYPE);
private:
    linkedlist* dobjdobj_list;
    ll_item* dobjdobj_item;
    dobjdobj* dobj_dobj;
};

#endif
#endif
