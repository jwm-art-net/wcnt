#ifndef DOBJ_H
#include "../include/dobj.h"

dobj::dobj(dobjnames::DOBJ_TYPE dt) :
	err_msg(0), object_type(dt), valid(true), username(0)
{
	err_msg = new string;
}

dobj::~dobj()
{
	if (username) 
		delete username;
	if (err_msg) 
		delete err_msg;
}

bool dobj::is_named_by_user()
{
	if (get_dobjnames()->check_type(object_type) == dobjnames::DOBJ_FIRST) {
		*err_msg = "\nError in object ";
		if (username) *err_msg += *username;
		*err_msg += " type ";
		*err_msg += dobj_names->get_name(object_type);
		return (valid = false);
	}
	// check_type(object_type) means need only check for 1 condition...
	if (object_type > dobjnames::DOBJ_DEFLISTS)
		return true;
	return false; // should not be named by user
}

bool dobj::set_username(const char* un)
{
	if (!is_named_by_user()) {// should not be given username
		*err_msg = "\ntried to set username of object type ";
		*err_msg += dobj_names->get_name(object_type);
		*err_msg += " - does not required naming";
		return (valid = false); 
	}
	if (un == 0) {
		*err_msg = "\nNULL username specified for object type ";
		*err_msg += dobj_names->get_name(object_type);
		return (valid = false);
	}
	if (strlen(un) == 0) {
		err_msg = new string;
		*err_msg = "\nNULL length username specified for object type ";
		*err_msg += dobj_names->get_name(object_type);
		return (valid = false);
	}
	username = new string(un);
	return true;
}

const string* dobj::get_username()
{
	if (get_dobjnames()->check_type(object_type) == dobjnames::DOBJ_FIRST)
		return 0;
	return username;
}

#ifndef BARE_MODULES
bool dobj::set_dparam(dparnames::DPAR_TYPE, void*)
{
	*err_msg = "\n***programmer error***\nattempt made to set parameter data";
	*err_msg += "\nof data object lacking capability to set them.";
	return 0;
}

void* dobj::get_dparam(dparnames::DPAR_TYPE)
{
	*err_msg = "\n***programmer error***\nattempt made to get parameter data";
	*err_msg += "\nfrom data object lacking capability to provide them.";
	return 0;
}

dobj* dobj::add_dobj(dobj*)
{
	*err_msg = "\n***programmer error***\nattempt made to add data object to";
	*err_msg +=	"\ndata object lacking ability to accept them";
	return 0;
}

iocat_names* dobj::iocatnames = 0;
dobjnames* dobj::dobj_names = 0;
dobjlist* dobj::dobj_list = 0;
dobjparamlist* dobj::dobjparam_list = 0;
dparnames* dobj::dpar_names = 0;
dobjdobjlist* dobj::dobj_dobjlist = 0;
#endif

#endif
