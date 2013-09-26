#ifndef MODOBJLIST_H
#define MODOBJLIST_H

#include "moddobj.h"
#include "linkedlist.h"
#include "listwork.h"

// behaves in similiar fashion to topdobjlist - see topdobjlist.h
// for some background...

class moddobjlist : public linked_list<moddobj>
{
public:
    moddobjlist();
    ~moddobjlist(){};

    moddobj* register_moddobj(
        synthmodnames::SYNTHMOD_TYPE, dobjnames::DOBJ_TYPE);

    moddobj* get_first_of_type(synthmodnames::SYNTHMOD_TYPE smt) {
        return (search_result =
            find_in_data(sneak_first(), search_type = smt)
                )->get_data();
    }

    moddobj* get_next_of_type() {
        return (search_result =
            find_in_data(search_result->get_next(), search_type)
                )->get_data();
    }

private:
    synthmodnames::SYNTHMOD_TYPE search_type;
    llitem* search_result;
};

#endif
