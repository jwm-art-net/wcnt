#ifndef DOBJDOBJ_H
#include "../include/dobjdobj.h"

#ifndef BARE_MODULES

dobjdobj::dobjdobj(dobjnames::DOBJ_TYPE dt, dobjnames::DOBJ_TYPE kid) 
	:	dobj_type(dt), sprog(kid)
{
}

dobjdobj::~dobjdobj()
{
}

#endif
#endif
