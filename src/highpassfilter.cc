#ifndef HIGHPASSFILTER_H
#include "../include/highpassfilter.h"

hpfilter::hpfilter(string uname) :
	synthmod(synthmodnames::MOD_HPFILTER, hpfilter_count, uname), 
	in_signal(0), in_deg_size(0), in_feedback(0), in_feed_mod(0), output(0),
	wet_output(0), feed_level(0), feed_modsize(0), wetdry(0), filter(0), 
	source(0), filterarraymax(0), fpos(0), filtertotal(0)
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
	source = new double[FILTERARRAYSIZE];
	for (int i = 0; i < FILTERARRAYSIZE; i++) {
		filter[i] = 0;
		source[i] = 0;
	}
	fpos = FILTERARRAYSIZE - 1;
	hpfilter_count++;
	validate();
	#ifndef BARE_MODULES
	create_params();
	#endif
}

hpfilter::~hpfilter() 
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
	delete [] filter;
	delete [] source;
}

#ifndef BARE_MODULES
void const* hpfilter::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot) {
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

void const* hpfilter::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it) {
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

bool hpfilter::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt) {
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

void hpfilter::run() 
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
		(filtertotal / filterarraymax) - source[fpos]) * wetdry;
	source[fpos] = *in_signal;
	filter[fpos] = *in_signal + *in_feedback * feedback;
	fpos--;
	if (fpos < 0) 
		fpos = FILTERARRAYSIZE - 1;
}

int hpfilter::hpfilter_count = 0;

#ifndef BARE_MODULES
bool hpfilter::done_params = false;

void hpfilter::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_HPFILTER, paramnames::PAR_FEED_LEVEL);
	get_paramlist()->add_param(synthmodnames::MOD_HPFILTER, paramnames::PAR_FEED_MODSIZE);
	get_paramlist()->add_param(synthmodnames::MOD_HPFILTER, paramnames::PAR_WETDRY);
	done_params = true;
}
#endif
#endif
