#ifndef TOPDOBJLIST_H
#include "../include/topdobjlist.h"
#include "../include/jwm_globals.h"
#include "../include/dobjdobjlist.h"
#include "../include/dobjdobj.h"

topdobjlist::topdobjlist() :
 search_result(0), search_type(dobjnames::DOBJ_FIRST)
{
}

topdobjlist::~topdobjlist()
{
}

dobjdobjlist* topdobjlist::create_dobjdobjlist(
    dobjnames::DOBJ_TYPE parent, dobjnames::DOBJ_TYPE child)
{
    if (jwm.get_dobjnames()->check_type(parent)
        == dobjnames::DOBJ_FIRST)
        return 0;
    if (jwm.get_dobjnames()->check_type(child)
        == dobjnames::DOBJ_FIRST)
        return 0;
    dobjdobjlist* newddlist = new dobjdobjlist;
    newddlist->add_dobjdobj(parent, child);
    if (!add_at_tail(newddlist)) {
        delete newddlist;
        return 0;
    }
    return newddlist;
}
#endif
