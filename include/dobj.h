#ifndef DOBJ_H
#define DOBJ_H

#include <iostream>

#include "dobjnames.h"
#include "dparnames.h"

// forward definitions
#ifndef BARE_MODULES
class dobjlist; // list of dobj's defined outside of a module
class dobjparamlist; // list of all dparams for all dobj's
class dobjdobjlist;
#endif

class dobj
{
 public:
	dobj(dobjnames::DOBJ_TYPE objtype);
	virtual ~dobj();
	dobjnames::DOBJ_TYPE get_object_type()
 		{ return ((this) ? object_type : dobjnames::DOBJ_FIRST); }
	bool is_named_by_user();
	bool set_username(const char*);
	const string* get_username();
	bool is_valid(){ return ((this) ? valid : false);}
	const string* get_error_msg(){ return ((this) ? err_msg : 0);}
	#ifndef BARE_MODULES
	virtual bool set_dparam(dparnames::DPAR_TYPE, void*);
	// get_dparam() creates new(whatever) and returns it as void*
	// so make sure you cast it to right type to delete it!
	virtual void* get_dparam(dparnames::DPAR_TYPE);
	virtual dobj* add_dobj(dobj*); // don't be fooled...
	static void register_iocatnames(iocat_names* in){iocatnames = in;}
	static void register_dobjnames(dobjnames* dn){ dobj_names = dn;}
	static void register_dobjlist(dobjlist* dl){ dobj_list = dl;}
	static void register_dparlist(dobjparamlist* dp){ dobjparam_list = dp;}
	static void register_dparnames(dparnames* dp){ dpar_names = dp;}
	static void register_dobjdobjlist(dobjdobjlist* dl){ dobj_dobjlist = dl;}
	static iocat_names* get_iocatnames(){ return iocatnames;}
	static dobjnames* get_dobjnames(){ return dobj_names;}
	static dobjlist* get_dobjlist(){ return dobj_list;}
	static dobjparamlist* get_dobjparamlist(){ return dobjparam_list;}
	static dparnames* get_dparnames(){ return dpar_names;}
	static dobjdobjlist* get_dobjdobjlist(){ return dobj_dobjlist;}
	#endif
 protected:
	#ifndef BARE_MODULES
	virtual void create_dparams() = 0;
	#endif
	virtual bool validate() = 0;
	void invalidate(){ valid = false;}
	string* err_msg;
 private:
	dobjnames::DOBJ_TYPE object_type;
 	bool valid;
 	string* username;
	#ifndef BARE_MODULES
	static iocat_names* iocatnames;
	static dobjnames* dobj_names;
	static dobjlist* dobj_list;
	static dobjparamlist* dobjparam_list;
	static dparnames* dpar_names;
	static dobjdobjlist* dobj_dobjlist;
	#endif
};

#endif
