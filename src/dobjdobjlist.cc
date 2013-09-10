#ifndef MODOBJLIST_H
#include "../include/dobjdobjlist.h"

#ifndef BARE_MODULES

dobjdobjlist::dobjdobjlist() :
	dobjdobj_list(0), dobjdobj_item(0), dobj_dobj(0)
{
	dobjdobj_list = new 
		linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

dobjdobjlist::~dobjdobjlist()
{
	// delete dobj_dobj items in list, 
	// leave deletion of list to the list destructor
	goto_first();
	while (dobj_dobj) {
		delete dobj_dobj;
		goto_next();
	}
	delete dobjdobj_list;
}

dobjdobj* dobjdobjlist::add_dobjdobj(dobjdobj* mo)
{
	return (dobjdobj*)
		(dobjdobj_item = dobjdobj_list->add_at_tail(mo))->get_data();
}

dobjdobj* 
dobjdobjlist::add_dobjdobj(dobjnames::DOBJ_TYPE dt, dobjnames::DOBJ_TYPE kid)
{
	if (dobj::get_dobjnames()->check_type(dt) == dobjnames::DOBJ_FIRST)
		return 0;
	if (dobj::get_dobjnames()->check_type(kid) == dobjnames::DOBJ_FIRST)
		return 0;
	dobjdobj* mo = new dobjdobj(dt, kid);
	ll_item* ll;
	if (!(ll = dobjdobj_list->add_at_tail(mo))) {
		delete mo;
		return 0;
	}
	return (dobj_dobj = (dobjdobj*)(dobjdobj_item = ll)->get_data());
}

dobjdobjlist* 
dobjdobjlist::get_dobjdobjlist_for_dobjtype(dobjnames::DOBJ_TYPE dt)
{
	if (dobj::get_dobjnames()->check_type(dt) == dobjnames::DOBJ_FIRST)
		return 0;
	dobjdobjlist* mdl = new dobjdobjlist();
	goto_first();
	while(dobj_dobj != 0) {
		if (dobj_dobj->get_dobj_type() == dt) {
			if (!mdl->add_dobjdobj(dt, dobj_dobj->get_dobj_sprog())) {
				delete mdl;
				return 0;
			}
		}
		goto_next();
	}
	return mdl;
}

dobjdobjlist* 
dobjdobjlist::get_dobjdobjlist_of_sprogs(dobjnames::DOBJ_TYPE kid)
{
	if (dobj::get_dobjnames()->check_type(kid) == dobjnames::DOBJ_FIRST)
		return 0; // check_type returns DOBJ_FIRST if dt not user type-------
	dobjdobjlist* mdl = new dobjdobjlist();
	goto_first();
	while(dobj_dobj != 0) {
		if (dobj_dobj->get_dobj_sprog() == kid) {
			if (!mdl->add_dobjdobj(dobj_dobj->get_dobj_type(), kid)) {
				delete mdl;
				return 0;
			}
		}
		goto_next();
	}
	return mdl;
}

#endif
#endif
