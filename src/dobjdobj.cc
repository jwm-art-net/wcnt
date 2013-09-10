#ifndef DOBJDOBJ_H
#include "../include/dobjdobj.h"

dobjdobj::dobjdobj(dobjnames::DOBJ_TYPE dt, dobjnames::DOBJ_TYPE kid) :
 dobj_type(dt), sprog(kid)
{
}
dobjdobj::dobjdobj(dobjdobj & rhs)
{
    dobj_type = rhs.get_dobj_type();
    sprog = rhs.get_dobj_sprog();
}

dobjdobj::~dobjdobj()
{
}

dobjdobj & dobjdobj::operator= (dobjdobj & rhs)
{
    if (this == &rhs)
        return *this;
    dobj_type = rhs.get_dobj_type();
    sprog = rhs.get_dobj_sprog();
    return *this;
}

#endif
