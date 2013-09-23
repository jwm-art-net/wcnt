#ifndef DOBJLIST_H
#define DOBJLIST_H

#include "dobjnames.h"
#include "linkedlist.h"
#include "listwork.h"

#include "dobj.h"

class dobjlist : public linked_list<dobj>
{
 public:
    dobjlist(): search_type(dobjnames::DOBJ_FIRST), search_result(0)
    {};

    dobj* add_dobj(dobj* d) { return add_at_tail(d)->get_data(); }

    // create_dobj does not add to list.
    dobj* create_dobj(dobjnames::DOBJ_TYPE);

    dobj* get_dobj_by_name(const char* n) {
        return find_in_data(sneak_first(), name(n))->get_data();
    }

    dobj* get_first_of_type(dobjnames::DOBJ_TYPE dt) {
        return (search_result =
            find_in_data(sneak_first(), search_type = dt))->get_data();
    }

    dobj* get_next_of_type() {
        return (search_result =
            find_in_data(search_result->get_next(),
                search_type))->get_data();
    }

 private:
    dobjnames::DOBJ_TYPE search_type;
    llitem* search_result;
};

#endif
