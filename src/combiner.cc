#ifndef COMBINER_H
#include "../include/combiner.h"

combiner::combiner(string uname) : 
	synthmod(synthmodnames::MOD_COMBINER, combiner_count, uname),
  	out_output(0), sigcount(0), meantotal(ON), wcntsiglist(0), 
	wcntsig_item(0), wcntsig(0)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	#endif
	wcntsiglist = new 
		linkedlist(linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
	combiner_count++;
	#ifndef BARE_MODULES
	create_params();
	create_moddobj();
	#endif
}

combiner::~combiner()
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
    if (wcntsiglist) {
		goto_first();
		while (wcntsig_item) {
			delete wcntsig;
			goto_next();
		}
		delete wcntsiglist;
	}
}

#ifndef BARE_MODULES
void const* combiner::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_OUTPUT:
			o = &out_output;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* combiner::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		default:
			i = 0;
	}
	return i;
}

bool combiner::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_MEAN_TOTAL:
			meantotal = *(STATUS*)data;
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

bool combiner::validate()
{
	if (!goto_first()) {
		*err_msg = "\nthere must be atleast two signals to combine";
		invalidate();
	} else if (!goto_next()){
		*err_msg = "\nthere must be atleast two signals to combine";
		invalidate();
	}
	return is_valid();
}

dobj* combiner::add_dobj(dobj* dbj)
{
	if (dbj->get_object_type() == dobjnames::DOBJ_SYNTHMOD) {
		synthmod* sm = ((dobjmod*)dbj)->get_synthmod();
		if (sm->get_module_type() != synthmodnames::MOD_WCNTSIGNAL) {
			*err_msg = "\n" + *sm->get_username() + 
				" is not a wcnt_signal";
			return 0;
		}
		if (!add_signal((wcnt_signal*)sm)) {
			*err_msg = "\ncould not insert " + *sm->get_username() +
				" into combiner";
			return 0;
		}
		// add the dobj synthmod wrapper to the dobjlist 
		// so it gets deleted in the end.
		dobj::get_dobjlist()->add_dobj(dbj);
		return dbj;
	}
	*err_msg = "\n***major error*** attempt made to add an ";
	*err_msg += "\ninvalid object type to " + *get_username();
	return 0;
}

#endif // BARE_MODULES

void combiner::init()
{
	goto_first(); 
	sig = wcntsig->get_output();
}

void combiner::run() 
{
	total = 0;
	goto_first();
	while(wcntsig){
		total += *(wcntsig->get_output());
		goto_next();
	}
	if (meantotal == ON)
		out_output = total / sigcount;
	else
		out_output = total;
}

int combiner::combiner_count = 0;

#ifndef BARE_MODULES
bool combiner::done_params = false;
void combiner::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->
		add_param(synthmodnames::MOD_COMBINER, paramnames::PAR_MEAN_TOTAL);
	done_params = true;
}

bool combiner::done_moddobj = false;
void combiner::create_moddobj()
{
	if (done_moddobj == true)
		return;
	get_moddobjlist()->
		add_moddobj(synthmodnames::MOD_COMBINER, dobjnames::LIN_SIGNALS);
	// several modules use LIN_SIGNALS, if they all add a dobjdobj to
	// it, then wcnt will want to read as many as has been added, if there
	// are more to be read after, then it will want to read that many
	// again.  glamorous eh?
	if (!dobj::get_dobjdobjlist()->
			get_dobjdobjlist_for_dobjtype(dobjnames::LIN_SIGNALS)->
				goto_first()) 
	{
		dobj::get_dobjdobjlist()->
			add_dobjdobj(dobjnames::LIN_SIGNALS, dobjnames::DOBJ_SYNTHMOD);
	}	
	done_moddobj = true; 
}

#endif
#endif
