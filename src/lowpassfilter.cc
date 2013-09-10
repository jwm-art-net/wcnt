#ifndef LOWPASSFILTER_H
#include "../include/lowpassfilter.h"

lpfilter::lpfilter(string uname) :
	synthmod(synthmodnames::MOD_LPFILTER, lpfilter_count, uname), 
	in_signal(0), in_deg_size(0), in_feedback(0), in_feed_mod(0), output(0),
	wet_output(0), feed_level(0), feed_modsize(0), wetdry(0), filter(0), 
	filterarraymax(0), fpos(0), filtertotal(0)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	get_outputlist()->add_output(this, outputnames::OUT_WET_OUTPUT);
	get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
	get_inputlist()->add_input(this, inputnames::IN_CUTOFF_DEG_SIZE);
	get_inputlist()->add_input(this, inputnames::IN_FEEDBACK);
	get_inputlist()->add_input(this, inputnames::IN_FB_MOD);
	#endif
	filter = new double[FILTERARRAYSIZE];
	for (int i = 0; i < FILTERARRAYSIZE; i++) filter[i] = 0;
	fpos = FILTERARRAYSIZE - 1;
	lpfilter_count++;
	validate();
	#ifndef BARE_MODULES
	create_params();
	#endif
}

lpfilter::~lpfilter() 
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
	delete [] filter;
}

#ifndef BARE_MODULES
void const* lpfilter::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_OUTPUT:
			o = &output;
			break;
		case outputnames::OUT_WET_OUTPUT:
			o = &wet_output;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* lpfilter::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_SIGNAL:
			i = in_signal = (double*)o;
			break;
		case inputnames::IN_CUTOFF_DEG_SIZE:
			i = in_deg_size = (double*)o;
			break;
		case inputnames::IN_FEEDBACK:
			i = in_feedback = (double*)o;
			break;
		case inputnames::IN_FB_MOD:
			i = in_feed_mod = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool lpfilter::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_FEED_LEVEL:
			set_feedback_level(*(double*)data); 
			retv = true;
			break;
		case paramnames::PAR_FEED_MODSIZE:
			set_feedback_modsize(*(double*)data); 
			retv = true;
			break;
		case paramnames::PAR_WETDRY:
			set_wetdry(*(double*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}
#endif // BARE_MODULES

void lpfilter::run() 
{
	filterarraymax = (short)(180 / *in_deg_size);
	if (filterarraymax < 2) filterarraymax = 2;
	else if (filterarraymax > FILTERARRAYSIZE)
		filterarraymax = FILTERARRAYSIZE - 1;
	feedback = feed_level * (1 - feed_modsize) +
		(feed_level * feed_modsize * *in_feed_mod);
	filtertotal = 0;
	for (int i = fpos; i < fpos + filterarraymax; i ++)
		filtertotal += filter[i % FILTERARRAYSIZE];
	output = *in_signal * (1 - wetdry) + (wet_output = 
		(filtertotal / filterarraymax)) * wetdry;
	filter[fpos] = *in_signal + *in_feedback * feedback;
	fpos--;
	if (fpos < 0) 
		fpos = FILTERARRAYSIZE - 1;
}

int lpfilter::lpfilter_count = 0;

#ifndef BARE_MODULES
bool lpfilter::done_params = false;

void lpfilter::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_LPFILTER, paramnames::PAR_FEED_LEVEL);
	get_paramlist()->add_param(synthmodnames::MOD_LPFILTER, paramnames::PAR_FEED_MODSIZE);
	get_paramlist()->add_param(synthmodnames::MOD_LPFILTER, paramnames::PAR_WETDRY);
	done_params = true;
}
#endif
#endif
