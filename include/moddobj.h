#ifndef MODDOBJ_H
#define MODDOBJ_H

#include "synthmodnames.h"
#include "dobjnames.h"

/*
// moddobj has been redesigned to contain a dobjdobjlist instead
// of a dobjnames::DOBJ_TYPE
//
// see topdobjlist.h for an explanation which with slight adjustment
// will explain this decision.
*/

class dobjdobjlist;

class moddobj
{
 public:
    moddobj(module::TYPE, dobjnames::DOBJ_TYPE);
    ~moddobj();
    module::TYPE get_moduletype() const {
        return (this) ? synthmodule_type : module::ERR_TYPE;
    }
    dobjnames::DOBJ_TYPE get_first_child() const { return first_child; }
    dobjdobjlist* get_dobjdobjlist() const { return (this) ? dd_list : 0;}

    bool operator()(module::TYPE & smt) const {
        return this->synthmodule_type == smt;
    }

 private:
    module::TYPE synthmodule_type;
    dobjnames::DOBJ_TYPE first_child;
    dobjdobjlist* dd_list;
};

#endif
