#ifndef DC_FILTER_H
#include "../include/dc_filter.h"

dc_filter::dc_filter(string uname) :
	synthmod(synthmodnames::MOD_DCFILTER, dc_filter_count, uname), 
	in_signal(0), output(0), dc_time(0), filter(0), filterarraymax(0),
	fpos(0), filtertotal(0)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
	#endif
	dc_filter_count++;
	validate();
	#ifndef BARE_MODULES
	create_params();
	#endif
}

dc_filter::~dc_filter() 
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
	delete [] filter;
}

void dc_filter::init()
{
	filterarraymax = (short)((dc_time * audio_samplerate) / 1000);
	filter = new double[filterarraymax];
	for (int i = 0; i < filterarraymax; i++) filter[i] = 0;
	fpos = filterarraymax - 1;
}

#ifndef BARE_MODULES
void const* dc_filter::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_OUTPUT:
			o = &output;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* dc_filter::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_SIGNAL:
			i = in_signal = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool dc_filter::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_DC_TIME:
			set_dc_time(*(double*)data); 
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}
#endif // BARE_MODULES

void dc_filter::run() 
{
	filtertotal = 0;
	for (int i = fpos; i < fpos + filterarraymax; i ++)
		filtertotal += filter[i % filterarraymax];
	output = (filtertotal / filterarraymax) - filter[fpos];
	filter[fpos] = *in_signal;
	fpos--;
	if (fpos < 0) 
		fpos = filterarraymax - 1;
}

int dc_filter::dc_filter_count = 0;

#ifndef BARE_MODULES
bool dc_filter::done_params = false;

void dc_filter::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_DCFILTER, paramnames::PAR_DC_TIME);
	done_params = true;
}
#endif
#endif
