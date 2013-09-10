#ifndef DOBJPARAM_H
#define DOBJPARAM_H

#include "dobj.h"

#ifndef BARE_MODULES

// ties object parameters to an object
// the knot is a reference to an object type, and a refernce to a dparam type.

class dobjparam
{
 public:
	dobjparam(dobjnames::DOBJ_TYPE dt, dparnames::DPAR_TYPE dpt);
	~dobjparam(){};
	dobjnames::DOBJ_TYPE get_dobjtype()
		{ return (this) ? dobjtype : dobjnames::DOBJ_FIRST;}
	dparnames::DPAR_TYPE get_dpartype()
		{ return (this) ? dpartype : dparnames::DPAR_FIRST;}
	IOCAT get_dparam_category()	{ return (this) 
		? dobj::get_dparnames()->get_category(dpartype) : CAT_FIRST;}
 private:
	dobjnames::DOBJ_TYPE dobjtype;
	dparnames::DPAR_TYPE dpartype;
};

#endif
#endif
