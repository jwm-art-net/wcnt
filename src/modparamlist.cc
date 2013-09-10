#ifndef MODPARAMLIST_H
#include "../include/modparamlist.h"

modparamlist::modparamlist() 
: parlist(0), param(0)
{
	if (!(parlist = new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA)))
	{
		cerr << "No nuff memree 4par lis";
		return;
	}
}

modparamlist::~modparamlist()
{
	ll_item* tmp = parlist->goto_first();
	while(tmp != 0)
	{
		delete (modparam*)tmp->get_data();
		tmp = parlist->goto_next();
	}
}

modparam* modparamlist::add_param(modparam* mp)
{
	// did have lookup_data_match(....) (linkedlist.h) here, but it can only check
	// one param at a time, so it's useless for check two like I wanted here.
	// leave it to synthmodules ok.....to know when it's already been done.
	return (modparam*) parlist->add_at_tail(mp)->get_data();
}

modparam* modparamlist::add_param(synthmodnames::SYNTH_MOD_TYPE smt, paramnames::PAR_TYPE pt)
{
	modparam* mp = new modparam(smt, pt);
	if (!mp) 
		return 0;
	ll_item* tmp = parlist->add_at_tail(mp);
	if (!tmp){
		delete mp;
		return 0;
	}
	return (modparam*)tmp->get_data();
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
	if (!mpl)
		return 0; //oh dear
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
