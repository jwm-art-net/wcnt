#ifndef MODDOBJ_H
#include "../include/moddobj.h"

#ifndef BARE_MODULES

moddobj::moddobj(synthmodnames::SYNTH_MOD_TYPE smt,	dobjnames::DOBJ_TYPE dt) 
	:	synthmodule_type(smt), dobj_type(dt)
{
}

moddobj::~moddobj()
{
}

#endif
#endif
