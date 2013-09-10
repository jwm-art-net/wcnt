#ifndef COMBINER_H
#include "../include/combiner.h"

combiner::combiner(string uname) : 
	synthmod(synthmodnames::MOD_COMBINER, combiner_count, uname),
  	out_output(0), sigcount(0), meantotal(ON), wcntsiglist(0), wcntsig_item(0), 
	wcntsig(0)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	#endif
	wcntsiglist = new linkedlist(
		linkedlist::MULTIREF_OFF, linkedlist::NO_NULLDATA);
	combiner_count++;
	validate();
	#ifndef BARE_MODULES
	create_params();
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
#endif // BARE_MODULES

void combiner::init()
{
	if (!goto_first()) {//check for atleast one signal
		invalidate();// no signals abort
		return;
	} else if (!goto_next()){// check for second
		invalidate();// one signal abort
		return; 
	} else // more than one so start at begining again
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
	get_paramlist()->add_param(synthmodnames::MOD_COMBINER, paramnames::PAR_MEAN_TOTAL);
	done_params = true;
}
#endif
#endif
