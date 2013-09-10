#ifndef SWITCHER_H
#include "../include/switcher.h"

switcher::switcher(string uname) 
: synthmod(synthmodnames::MOD_SWITCHER, switcher_count, uname),
  in_trig(0), out_output(0), xfadetime(25), xfade_samp(0), xfade_max_samps(0),
  xfade_stpsz(0), xfade_size(0), wcntsiglist(0), wcntsig_item(0), wcntsig(0), sig(0), prevsig(0)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	get_inputlist()->add_input(this, inputnames::IN_TRIG);
	#endif
	wcntsiglist = 
		new linkedlist(linkedlist::MULTIREF_ON, linkedlist::NO_NULLDATA);
	switcher_count++;
	validate();
	#ifndef BARE_MODULES
	create_params();
	#endif
	
}

switcher::~switcher()
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
	goto_first();
	while (wcntsig_item) {
		delete wcntsig;
		goto_next();
	}
	delete wcntsiglist;
}

#ifndef BARE_MODULES
void const* switcher::get_out(outputnames::OUT_TYPE ot)
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

void const* switcher::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_TRIG:
			i = in_trig = (STATUS*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool switcher::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_XFADE_TIME:
			xfadetime = *(double*)data;
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}
#endif // BARE_MODULES

void switcher::init()
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
	xfade_samp = xfade_max_samps = convert_ms_to_samples(xfadetime);
	xfade_stpsz = 1 / (double)xfade_samp;
	xfade_size = 0;
	prevsig = &zero;
}

void switcher::run() 
{
	if (*in_trig == ON) {
		prevsig = sig;
		if (!(wcntsig = goto_next()))
			wcntsig = goto_first();
		sig = wcntsig->get_output();
		xfade_samp = xfade_max_samps;
		xfade_size = 0;
	}
	if (xfade_samp > 0) {
		out_output = *prevsig * (1 - xfade_size) + *sig * xfade_size;
		xfade_samp--;
		xfade_size += xfade_stpsz;
	} else
		out_output = *sig;
}

int switcher::switcher_count = 0;

#ifndef BARE_MODULES
bool switcher::done_params = false;

void switcher::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_SWITCHER, paramnames::PAR_XFADE_TIME);
	done_params = true;
}
#endif
#endif
