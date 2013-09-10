#ifndef DOBJLIST_H
#include "../include/dobjlist.h"

#ifndef BARE_MODULES

dobjlist::dobjlist() :
	dobj_list(0), dobj_item(0), d_obj(0), 
	search_dobj_type(dobjnames::DOBJ_FIRST), dobj_search(0)
{
	dobj_list = new 
		linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
}

dobjlist::~dobjlist()
{
	goto_first();
	while(d_obj) {
		delete d_obj;
		goto_next();
	}
	delete dobj_list;
}

dobj* dobjlist::add_dobj(dobj* d)
{
	if (!(dobj_item = dobj_list->add_at_tail(d)))
		return 0;
	return (d_obj = (dobj*)dobj_item->get_data());
}

dobj* dobjlist::create_dobj(dobjnames::DOBJ_TYPE dt)
{
	if (dobj::get_dobjnames()->check_type(dt) == dobjnames::DOBJ_FIRST)
		return 0;
	dobj* dobjy = 0;
	switch(dt) {
		// note: LIN_xxx types are not created here as they are
		// encapsulated by the synth module, ie cannot be created
		// seperately from it.
		case dobjnames::SIN_NOTE:
			dobjy = new note_data;
			break;
		case dobjnames::SIN_COORD:
			dobjy = new adsr_coord;
			break;
		case dobjnames::SIN_VERTEX:
			dobjy = new wave_vertex;
			break;
		case dobjnames::SIN_METER:
			dobjy = new meterchange;
			break;
		case dobjnames::SIN_BPM:
			dobjy = new bpmchange;
			break;
		case dobjnames::SIN_RIFFNODE:
			dobjy = new riff_node;
			break;
		case dobjnames::SIN_DVERTEX:
			dobjy = new dynvertex;
			break;
		case dobjnames::LDEF_RIFF:
			dobjy = new riffdata;
			break;
		case dobjnames::LDEF_SAMPLESET:
//			dobjy = new sampleset(*uname);
			break;
		case dobjnames::LDEF_RULESET:
//			dobjy = new ruleset(*uname);
			break;
		case dobjnames::SDEF_WAVFILEIN:
			dobjy = new wavfilein;
			break;
		case dobjnames::SDEF_SAMPLEDATA:
			dobjy = new sample_data;
			break;
		case dobjnames::SDEF_MAPRULE:
//			dobjy = new maprule;
			break;
		case dobjnames::DOBJ_SYNTHMOD:
			dobjy = new dobjmod;
			break;
		default:
			dobjy = 0;
	}
	return dobjy;
}

dobj* dobjlist::get_dobj_by_name(const string* dname)
{
	goto_first();
	while(d_obj) {
		if (*d_obj->get_username() == *dname)
			return d_obj;
		goto_next();
	}
	return 0;
}

dobj* dobjlist::get_first_of_dobj_type(dobjnames::DOBJ_TYPE dt)
{
	dobj_search = 0;
	search_dobj_type = dobjnames::DOBJ_FIRST;
	if (dobj::get_dobjnames()->check_type(dt) == dobjnames::DOBJ_FIRST)
		return 0;
	goto_first();
	while(d_obj) {
		if (d_obj->get_object_type() == dt) {
			dobj_search = dobj_item;
			search_dobj_type = dt;
			return d_obj;
		}
		goto_next();
	}
	return 0;
}

dobj* dobjlist::get_next_of_dobj_type()
{
	if (dobj::get_dobjnames()->check_type(search_dobj_type) 
			== dobjnames::DOBJ_FIRST)
		return 0;
	dobj_item = dobj_list->goto_item(dobj_search);
	goto_next();
	while(d_obj) {
		if (d_obj->get_object_type() == search_dobj_type) {
			dobj_search = dobj_item;
			return d_obj;
		}
		goto_next();
	}
	dobj_search = 0;
	search_dobj_type = dobjnames::DOBJ_FIRST;
	return 0;
}
		
#endif
#endif
