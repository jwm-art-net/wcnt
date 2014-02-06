#ifndef FXSPARAMLIST_H
#define FXSPARAMLIST_H

#include "linkedlist.h"
#include "fixstrparam.h"
#include "listwork.h"

class fxsparamlist : public linked_list<fixstrparam>
{
 public:
    fxsparamlist(){};

    fixstrparam* add_param(const char* string_list, int param_type);

    fixstrparam* get_fix_str_param(int param_type)
    {
        return find_in_data(sneak_first(), param_type)->get_data();
    }

    #ifdef DEBUG
    void dump();
    #endif

};

#endif
