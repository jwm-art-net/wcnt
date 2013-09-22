#ifndef DOBJDOBJLIST_H
#define DOBJDOBJLIST_H


#include "linkedlist.h"
#include "dobjnames.h"
#include "dobjdobj.h"

/*
// good name for a class I think, some may find it ugly, but beauty is
// in the eye of the beholder. that's what I keep telling myself anyway.
*/

class dobjdobjlist : public linked_list<dobjdobj>
{
 public:
    dobjdobjlist(){};
    dobjdobjlist(DESTRUCTION d): linkedlist(MULTIREF_OFF, d) {};


    dobjdobj* register_dobjdobj(dobjdobj* dd) {
        return add_at_tail(dd)->get_data();
    }

    dobjdobj* register_dobjdobj(dobjnames::DOBJ_TYPE, dobjnames::DOBJ_TYPE);

    dobjdobjlist* get_dobjdobjlist_for_dobjtype(dobjnames::DOBJ_TYPE);
    #ifdef UNUSED
    dobjdobjlist* get_dobjdobjlist_of_sprogs(dobjnames::DOBJ_TYPE);
    #endif
};

#endif
