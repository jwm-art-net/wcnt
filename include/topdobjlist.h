#ifndef TOPDOBJLIST_H
#define TOPDOBJLIST_H

#include "linkedlist.h"
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
// using the same dobj::TYPE.)

class topdobjlist : public linked_list<dobjdobjlist>
{
public:
    topdobjlist();
    ~topdobjlist();
    dobjdobjlist* create_dobjdobjlist(dobj::TYPE parent,
                                        dobj::TYPE child);

    dobjdobjlist* get_first_of_type(dobj::TYPE dt) {
        search_type = dt;
        search_result = sneak_first();
        while(search_result) {
            dobjdobjlist* dd_list = search_result->get_data();
            if (dd_list->sneak_first()->get_data()
                        ->get_dobj_type() == search_type)
                return dd_list;
            search_result = search_result->get_next();
        }
        search_result = 0;
        return 0;
    }

    dobjdobjlist* get_next_of_type() {
        search_result = search_result->get_next();
        while(search_result) {
            dobjdobjlist* dd_list = search_result->get_data();
            if (dd_list->sneak_first()->get_data()
                        ->get_dobj_type() == search_type)
                return dd_list;
            search_result = search_result->get_next();
        }
        search_result = 0;
        return 0;
    }

private:
    llitem* search_result;
    dobj::TYPE search_type;
};





#endif
