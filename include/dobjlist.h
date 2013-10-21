#ifndef DOBJLIST_H
#define DOBJLIST_H

#include "dobjnames.h"
#include "linkedlist.h"
#include "listwork.h"

#include "dobj.h"

namespace dobj
{
 class list : public linked_list<dobj::base>
 {
  public:
    list(): search(dobj::ERR_TYPE), result(0)
    {};

    dobj::base* add_dobj(dobj::base* d) {
        return add_at_tail(d)->get_data();
    }

    // create_dobj does not add to list.
    static dobj::base* create_dobj(dobj::TYPE);

    dobj::base* get_dobj_by_name(const char* n) {
        return find_in_data(sneak_first(), name(n))->get_data();
    }

    dobj::base* get_first_of_type(dobj::TYPE dt) {
        result = find_in_data(sneak_first(), search = dt);
        return result->get_data();
    }

    dobj::base* get_next_of_type() {
        result = find_in_data(result->get_next(), search);
        return result->get_data();
    }

  private:
    dobj::TYPE search;
    llitem* result;
 };
}; // namespace dobj

#endif
