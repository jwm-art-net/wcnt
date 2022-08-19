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
		llitem* tmp = find_in_data(sneak_first(), pt);
        return tmp ? tmp->get_data() : 0;
    }

    #ifdef DEBUG
    void dump();
    #endif

};

#endif
