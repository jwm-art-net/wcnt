#ifndef MODPARAMLIST_H
#include "../include/modparamlist.h"

#ifndef BARE_MODULES

modparamlist::modparamlist() 
: parlist(0), param(0)
{
	parlist = new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

modparamlist::~modparamlist()
{
	goto_first();
	while(param != 0) {
		delete param;
		goto_next();
	}
}

modparam* modparamlist::add_param(modparam* mp)
{
	return (modparam*) parlist->add_at_tail(mp)->get_data();
}

modparam* modparamlist::add_param(synthmodnames::SYNTH_MOD_TYPE smt, paramnames::PAR_TYPE pt)
{
	modparam* mp = new modparam(smt, pt);
	if (!parlist->add_at_tail(mp)) {
		delete mp;
		return 0;
	}
	return mp;
}

modparamlist* modparamlist::get_paramlist_for_moduletype(synthmodnames::SYNTH_MOD_TYPE smt)
{
	if (smt == synthmodnames::MOD_FIRST)
		return 0;
/*	if (parlist->is_empty())
		return 0;
	do not return 0 if parlist->is_empty().  The list will be empty if there is
	just modules created which do not contain any parameters.  This is not an error */
	modparamlist* mpl = new modparamlist;
	goto_first();
	while(param) {
		if (param->get_moduletype() == smt) 
			if (!mpl->add_param(param->get_moduletype(), param->get_paramtype())) {
				delete mpl;
				return 0;
			}
		goto_next();
	}
	return mpl;
}

#endif
#endif
