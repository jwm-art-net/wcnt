#ifndef TOPDOBJLIST_H
#include "../include/topdobjlist.h"
#include "../include/jwm_globals.h"
#include "../include/dobjdobjlist.h"
#include "../include/dobjdobj.h"

topdobjlist::topdobjlist() :
 tldobj_list(0), tldobj_item(0), dd_list(0)
{
    tldobj_list = 
     new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

topdobjlist::~topdobjlist()
{
    // delete dobj_dobj data in list,
    // leave deletion of list items to list destructor
    goto_first();
    while (dd_list) {
        delete dd_list;
        goto_next();
    }
    delete tldobj_list;
}

dobjdobjlist* topdobjlist::create_dobjdobjlist(
    dobjnames::DOBJ_TYPE parent, dobjnames::DOBJ_TYPE child)
{
    if (jwm.get_dobjnames().check_type(parent)
        == dobjnames::DOBJ_FIRST) return 0;
    if (jwm.get_dobjnames().check_type(child)
        == dobjnames::DOBJ_FIRST) return 0;
    dobjdobjlist* newddlist = new dobjdobjlist;
    newddlist->add_dobjdobj(parent, child);
    tldobj_item = tldobj_list->add_at_tail(newddlist);
    return dd_list = (dobjdobjlist*)tldobj_item->get_data();
}

dobjdobjlist* topdobjlist::get_first_of_type(dobjnames::DOBJ_TYPE dt)
{
    search_dbjtype = dt;
    search_result = 0;
    if (jwm.get_dobjnames().check_type(dt) == dobjnames::DOBJ_FIRST)
        return 0;
    goto_first();
    while (dd_list) {
        dobjdobj* dbj = dd_list->goto_first();
        if (dbj->get_dobj_type() == search_dbjtype)
            return dd_list = (dobjdobjlist*)
                (search_result = tldobj_item)->get_data();
        goto_next();
    }
    return 0;
}

dobjdobjlist* topdobjlist::get_next_of_type()
{
    if (search_result == 0)
        return 0;
    dd_list = (dobjdobjlist*)(search_result->get_data());
    goto_next(); // !otherwise it get's stuck..
    while(dd_list) {
        dobjdobj* dbj = dd_list->goto_first();
        if (dbj->get_dobj_type() == search_dbjtype) {
            return dd_list = (dobjdobjlist*)
                (search_result = tldobj_item)->get_data();
        }
        goto_next();
    }
    search_result = 0;
    return 0;
}

#endif
