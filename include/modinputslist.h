#ifndef MODINPUTSLIST_H
#define MODINPUTSLIST_H

#include "synthmodule.h"
#include "linkedlist.h"
#include "inputnames.h"
#include "modinput.h"

class modinputlist
{
 public:
	modinputlist();
	~modinputlist();
	modinput* add_input(synthmod*, inputnames::IN_TYPE);
	bool delete_module_inputs(synthmod*);
	modinput* goto_first(){ return input = (modinput*)(input_item = inlist->goto_first())->get_data();}
	modinput* goto_last(){ return input = (modinput*)(input_item = inlist->goto_last())->get_data();}
	modinput* goto_prev(){ return input = (modinput*)(input_item = inlist->goto_prev())->get_data();}
	modinput* goto_next(){ return input = (modinput*)(input_item = inlist->goto_next())->get_data();}
	modinputlist* get_list_of_category(IOCAT);
	modinputlist* get_list_of_category_orderpref(
		IOCAT in_cat, 
		synthmodnames::SYNTH_MOD_TYPE * sm_prefs,	/* NULL terminated array of modules to list first */
		inputnames::IN_TYPE* in_prefs, 			/* NULL terminated array of input types to list first */
		inputnames::IN_TYPE* not_in_prefs); /* NULL terminated array of input types to exclude */
	modinputlist* get_inputlist_for_module(synthmod*);
 private:
	bool delete_input(modinput*);
	linkedlist* inlist;
	ll_item* input_item;
	modinput* input;
};

#endif
