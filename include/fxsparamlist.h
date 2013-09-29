#ifndef FXSPARAMLIST_H
#define FXSPARAMLIST_H

#include "linkedlist.h"
#include "fixstrparam.h"
#include "listwork.h"

class fxsparamlist : public linked_list<fixstrparam>
{
 public:
    fxsparamlist(){};

    fixstrparam* add_param(const char* string_list, param::TYPE);

    fixstrparam* get_fix_str_param(param::TYPE pt)
    {
        return find_in_data(sneak_first(), pt)->get_data();
    }
};

#endif
