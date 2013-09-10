#ifndef TOPDOBJLIST_H
#define TOPDOBJLIST_H

#include "dobjdobjlist.h"

// a doubly linked list of doubly linked lists
//--------------------------------------------
//
// before topdobjlist:
//
//      a dobj based class such as a riff adds a dobjdobj with it's
//      top level dobj type DEF_RIFF to the main dobjdobjlist so that
//      when a riff comes to be created the synthfilereader finds the
//      dobjdobj which contains DEF_RIFF.  the dobjdobj child of DEF_RIFF
//      is LST_NOTES.  The synthfilereader then requests the dobjdobj
//      (which the riff also created) which has LST_NOTES as a parent.
//      The child type of that is SIN_NOTE.
//
//      However, if another dobj based class is created before the riff
//      and uses LST_NOTES with a child type other than SIN_NOTE, then
//      LST_NOTES is already taken and when the synthfilereader comes
//      to read a riff it will use the first LST_NOTES dobjdobj.
//
// topdobjlist removes this problem by having a seperate dobjdobjlist
// for every top level dobj.  this ties the list relationship for a top
// level dobj down to it's smallest components while allowing other top
// level dobj to use the same dobj type with different components.
// (but obviously you cannot have two different dobj based classes
// using the same dobjnames::DOBJ_TYPE.)

class topdobjlist
{
public:
    topdobjlist();
    ~topdobjlist();
    dobjdobjlist* create_dobjdobjlist(dobjnames::DOBJ_TYPE parent,
                                        dobjnames::DOBJ_TYPE child);
    dobjdobjlist* goto_first() {
        return dd_list = (dobjdobjlist*)
         (tldobj_item = tldobj_list->goto_first())->get_data();
    }
    dobjdobjlist* goto_last() {
        return dd_list = (dobjdobjlist*)
         (tldobj_item = tldobj_list->goto_last())->get_data();
    }
    dobjdobjlist* goto_prev() {
        return dd_list = (dobjdobjlist*)
         (tldobj_item = tldobj_list->goto_prev())->get_data();
    }
    dobjdobjlist* goto_next() {
        return dd_list = (dobjdobjlist*)
         (tldobj_item = tldobj_list->goto_next())->get_data();
    }
    dobjdobjlist* get_first_of_type(dobjnames::DOBJ_TYPE);
    dobjdobjlist* get_next_of_type();

private:
    linkedlist* tldobj_list;
    ll_item* tldobj_item;
    dobjdobjlist* dd_list;
    ll_item* search_result;
    dobjnames::DOBJ_TYPE search_dbjtype;
};

#endif
