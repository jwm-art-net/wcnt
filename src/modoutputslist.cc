#ifndef MODOUTPUTSLIST_H
#include "../include/modoutputslist.h"

modoutputlist::modoutputlist() 
: outlist(0), output(0)
{
	if (!(outlist = new linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA)))
	{
		cerr << "No nuff mearm ri 4 oot lis";
		exit(-1);
	}
}

modoutputlist::~modoutputlist()
{
	ll_item* tmp = outlist->goto_first();
	while(tmp)
	{
		delete (modoutput*)tmp->get_data();
		tmp = outlist->goto_next();
	}
}

modoutput* modoutputlist::add_output(synthmod* sm, outputnames::OUT_TYPE ot)
{
	modoutput* mo = new modoutput(sm, ot);
	if (!mo) 
		return 0;
	if (!outlist->add_at_tail(mo)){
		delete mo;
		return 0;
	}
	return mo;
}

void modoutputlist::delete_module_outputs(synthmod* sm)
{
	if (outlist->is_empty()) return;
	output = goto_first();
	while(output != 0)
	{
		if (output->getsynthmodule() == sm)
		{
			delete_output(output);
			output = goto_first();
		}
		output = goto_next();
	}
}

void modoutputlist::delete_output(modoutput* mo)
{	
	if (mo == 0) 
		return;
	ll_item * tmp = outlist->find_data(mo);
	if (tmp != 0)
	{
		outlist->unlink_item(tmp);
		delete mo;
		delete tmp;
	}
}

modoutputlist* modoutputlist::list_of_category(IOCAT oc)
{
	if (outlist->is_empty()) 
		return 0;
	if (oc <= CAT_FIRST || oc >= CAT_LAST) 
		return 0;
	output = goto_first();
	modoutputlist* outcatlist = new modoutputlist;
	outputnames* outnames = synthmod::get_outputnames();
	outputnames::OUT_TYPE ot;
	while(output != 0)
	{
		ot = output->getoutputtype();
		if (outnames->getcategory(ot) == oc)
			outcatlist->add_output(output->getsynthmodule(), ot);
		output = goto_next();
	}
	return outcatlist;
}

modoutputlist* modoutputlist::list_of_category_orderpref(IOCAT out_cat, 
		synthmodnames::SYNTH_MOD_TYPE* sm_prefs,
		outputnames::OUT_TYPE* out_prefs,
		outputnames::OUT_TYPE* not_out_prefs )
{
	if (outlist->is_empty())
		return 0;
	if (out_cat <= CAT_FIRST || out_cat >= CAT_LAST) 
		return 0;
	modoutputlist* pot_outs = list_of_category(out_cat);
	if (pot_outs == 0) 
		return 0; 
	modoutputlist* sorted_outs = new modoutputlist;
	if (sorted_outs == 0)
		return pot_outs;
	short op_count = 0;
	short nop_count = 0;
	outputnames::OUT_TYPE* op = out_prefs;
	while (*op > outputnames::OUT_FIRST && *op < outputnames::OUT_LAST) 
	{
		op_count++;
		op++;
	}
	outputnames::OUT_TYPE* nop = not_out_prefs;
	while (*nop > outputnames::OUT_FIRST && *nop < outputnames::OUT_LAST)
	{
		nop_count++;
		nop++;
	}
	short a,b,c;
	for (a = 0; a < op_count; a++)
	{
		for (b = 0; b < nop_count; b++)
		{
			if (out_prefs[a] == not_out_prefs[b]) 
			{
				out_prefs[a] = outputnames::OUT_FIRST;	/* invalid name output type preference */
				not_out_prefs[b] = outputnames::OUT_FIRST; 
			}
		}
	}
	synthmod * sm;	/* make list of outputs from pot_outs sorted with preferences. */
	for(a = 0; out_prefs[a] > outputnames::OUT_FIRST && out_prefs[a] < outputnames::OUT_LAST; a++)
	{
		for(c = 0; sm_prefs[c] > synthmodnames::MOD_FIRST && sm_prefs[c] < synthmodnames::MOD_LAST; c++)
		{
			output = pot_outs->goto_first();
			while(output != 0) 
			{
				sm = output->getsynthmodule();
				if (out_prefs[a] == output->getoutputtype()	&& sm_prefs[c] == sm->get_module_type())
					sorted_outs->add_output(sm, output->getoutputtype());
				output = pot_outs->goto_next();
			}
		}
	}
	short out_prefchk; /* add remaining valid outputs */
	short not_prefchk;
	output = pot_outs->goto_first();
	while(output != 0) 
	{
		out_prefchk = 0;
		for(a = 0; a < op_count; a++)
		{
			for(c = 0; sm_prefs[c] > synthmodnames::MOD_FIRST && sm_prefs[c] < synthmodnames::MOD_LAST; c++)
			{
				sm = output->getsynthmodule();
				if (out_prefs[a] == output->getoutputtype() && sm_prefs[c] == sm->get_module_type()) 
					out_prefchk++;
			}
		}
		not_prefchk = 0;
		for (short b = 0; b < nop_count; b++)
		{
			if (not_out_prefs[b] == output->getoutputtype())
				not_prefchk++;
		}
		if (out_prefchk == 0 && not_prefchk == 0) 
				sorted_outs->add_output(output->getsynthmodule(), output->getoutputtype());
		output = pot_outs->goto_next();
	}
	delete pot_outs;
	return sorted_outs;
}

modoutputlist* modoutputlist::get_outputlist_for_module(synthmod* sm)
{
	if (!sm)
		return 0;
	modoutputlist* mol = new modoutputlist;
	if (!mol)
		return 0; // bag o' shite
	goto_first();
	while(output) {
		if (output->getsynthmodule() == sm) {
			if (!mol->add_output(output->getsynthmodule(), output->getoutputtype())){
				delete mol;
				return 0;
			}
		}
		goto_next();
	}
	return mol;
}


bool modoutputlist::is_registered(synthmod* mod)
{
	if (!mod)
		return 0;
	goto_first();
	while(output) {
		if (output->getsynthmodule() == mod)
			return true;
	}
	return false;
}

#endif
