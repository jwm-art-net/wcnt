#ifndef DOBJPARAMLIST_H
#define DOBJPARAMLIST_H

#include "dobjparam.h"
#include "linkedlist.h"
#include "listwork.h"

/*
// holds a list of parameters for every dobj created
// which has parameters... see also: jwm_globals.h
*/

class dobjparamlist : public linked_list<dobjparam>
{
 public:
    dobjparamlist(){};
    dobjparamlist(DESTRUCTION d) : linkedlist(MULTIREF_OFF, d) {};

    dobjparam* add_dobjparam(dobjparam* dp)
        { return add_at_tail(dp)->get_data(); };

    dobjparam* add_dobjparam(dataobj::TYPE, param::TYPE);

    bool validate(dobj*, param::TYPE, errors::TYPE);
};

#endif
