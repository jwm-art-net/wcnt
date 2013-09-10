#ifndef MODPARAMLIST_H
#define MODPARAMLIST_H

#ifndef BARE_MODULES

#include "linkedlist.h"
#include "modparam.h"

/*
	modparamlist is slightly different to modinputlist or modoutputlist
	instead of taking a synthmod* arguement it takes 
	synthmodnames::SYNTH_MOD_TYPE.  The list also differs in
	the way it is called, ie the params should be added only once for
	each module type, not for every module created.  (possibly by a static 
	member func called by the module constructor)
	also, there is no need to delete the params once corresponding module
	has been deleted, can leave it till the end when the list is destroyed.
	Plus the fact I've removed the remove_module_... member funcs cos
	there's no need for them.
	futher, the list_of_category.... functions in modinputlist etc have been
	removed as there is no need to use them.  
	All of this is because the params are used in a completely different way
	to the inputs and outputs.
*/

class modparamlist
{
 public:
	modparamlist();
	~modparamlist();
	modparam* add_param(modparam*);
	modparam* add_param(synthmodnames::SYNTH_MOD_TYPE, paramnames::PAR_TYPE);
	modparam* goto_first(){ return param = (modparam*)(param_item = parlist->goto_first())->get_data();}
	modparam* goto_last(){ return param = (modparam*)(param_item = parlist->goto_last())->get_data();}
	modparam* goto_prev(){ return param = (modparam*)(param_item = parlist->goto_prev())->get_data();}
	modparam* goto_next(){ return param = (modparam*)(param_item = parlist->goto_next())->get_data();}
	modparamlist* get_paramlist_for_moduletype(synthmodnames::SYNTH_MOD_TYPE);
 private:
	bool delete_param(modparam*);
	linkedlist* parlist;
	ll_item* param_item;
	modparam* param;
};

#endif
#endif
