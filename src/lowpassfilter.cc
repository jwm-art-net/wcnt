#ifndef LOWPASSFILTER_H
#include "../include/lowpassfilter.h"

lpfilter::lpfilter(string uname)
:synthmod(synthmodnames::MOD_LPFILTER, lpfilter_count, uname), 
output(0.00), in_signal(NULL), in_deg_size(NULL), in_resampmod(NULL), k(0), filter(NULL),
filterarraymax(0), fpos(0), fp(0), filtertotal(0.00), resdeg(0.00), resdegstsz(0.00), resamount(0.00), resamp_modsize(0.00)
{
	if (!get_outputlist()->add_output(this, outputnames::OUT_OUTPUT)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_SIGNAL)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_CUTOFF_DEG_SIZE)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_RES_AMP_MOD)){
		invalidate();
		return;
	}
	if (!(filter = new double[FILTERARRAYSIZE])){
		invalidate();
		return;
	}
	for (fp = 0; fp < FILTERARRAYSIZE; fp++)filter[fp] = 0;
	fpos = FILTERARRAYSIZE - 1;
	lpfilter_count++;
	validate();
	create_params();
}

lpfilter::~lpfilter() 
{
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	if (filter)
		delete[] filter;
}

void const* lpfilter::get_out(outputnames::OUT_TYPE ot)
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
		case inputnames::IN_RES_AMP_MOD:
			i = in_resampmod = (double*)o;
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
		case paramnames::PAR_RES_LEVEL:
			set_resonance_level(*(double*)data); 
			retv = true;
			break;
		case paramnames::PAR_RES_AMP_MODSIZE:
			set_resonant_amp_modsize(*(double*)data); 
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

void lpfilter::run() 
{
	filterarraymax = (short)(180 / *in_deg_size);
	if (filterarraymax > FILTERARRAYSIZE)
		filterarraymax = FILTERARRAYSIZE;
	filter[fpos] = *in_signal + (*in_signal * resamount * sin(resdeg * DTR) * (resamp_modsize * *in_resampmod));
	for (fp = fpos, filtertotal = 0; fp < fpos + filterarraymax; fp ++)
		filtertotal += filter[fp % FILTERARRAYSIZE];
	output = filtertotal / filterarraymax;
	resdeg += *in_deg_size;
	if (resdeg >= 360) 
		resdeg -= 360;
	fpos--;
	if (fpos < 0) 
		fpos = FILTERARRAYSIZE - 1;
}

int lpfilter::lpfilter_count = 0;
bool lpfilter::done_params = false;

void lpfilter::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_LPFILTER, paramnames::PAR_RES_LEVEL);
	get_paramlist()->add_param(synthmodnames::MOD_LPFILTER, paramnames::PAR_RES_AMP_MODSIZE);
	done_params = true;
}

#endif
