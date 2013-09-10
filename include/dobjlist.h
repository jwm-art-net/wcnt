#ifndef DOBJLIST_H
#define DOBJLIST_H

#include "dobj.h"
#include "linkedlist.h"
#include "dataobjects.h"

#ifndef BARE_MODULES

class dobjlist
{
public:
    dobjlist();
    ~dobjlist();
    dobj* add_dobj(dobj*); // add a dobj to the list
    // create_dobj does not add to list.
    dobj* create_dobj(dobjnames::DOBJ_TYPE);
    dobj* goto_first() {
        return d_obj = (dobj*)
         (dobj_item = dobj_list->goto_first())->get_data();
    }
    dobj* goto_last() {
        return d_obj = (dobj*)
         (dobj_item = dobj_list->goto_last())->get_data();
    }
    dobj* goto_prev() {
        return d_obj = (dobj*)
         (dobj_item = dobj_list->goto_prev())->get_data();
    }
    dobj* goto_next() {
        return d_obj = (dobj*)
         (dobj_item = dobj_list->goto_next())->get_data();
    }
    dobj* get_dobj_by_name(char const*);
    dobj* get_first_of_dobj_type(dobjnames::DOBJ_TYPE);
    dobj* get_next_of_dobj_type();

private:
    linkedlist* dobj_list;
    ll_item* dobj_item;
    dobj*	d_obj;
    dobjnames::DOBJ_TYPE search_dobj_type;
    ll_item* dobj_search;
};
#endif
#endif
