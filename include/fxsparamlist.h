#ifndef FXSPARAMLIST_H
#define FXSPARAMLIST_H

#include "linkedlist.h"
#include "fixstrparam.h"
#include "listwork.h"

class fxsparamlist : public linked_list<fixstrparam>
{
 public:
    fxsparamlist(){};

    fixstrparam* add_param(const char* string_list, paramnames::PAR_TYPE);

    fixstrparam* get_fix_str_param(paramnames::PAR_TYPE pt)
    {
        return find_in_data(sneak_first(), pt)->get_data();
    }
};

#endif
