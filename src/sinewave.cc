#ifndef SINEWAVE_H
#include "../include/sinewave.h"

sine_wave::sine_wave(string uname)
:synthmod(synthmodnames::MOD_SINEWAVE, sine_wave_count, uname),
output(0.00), play_state(OFF), in_phase_trig(NULL), in_deg_size(NULL), degs(0.00), recycle(OFF), zero_deg(OFF),
cycles(1.00), maxdegs(360)
{
	if (!get_outputlist()->add_output(this, outputnames::OUT_OUTPUT)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_PHASE_TRIG)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_DEG_SIZE)){
		invalidate();
		return;
	}
	sine_wave_count++;
	validate();
	create_params();
}
sine_wave::~sine_wave() 
{
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
}

void const* sine_wave::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_OUTPUT:
			o = &output;
			break;
		case outputnames::OUT_PLAY_STATE:
			o = &play_state;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* sine_wave::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_PHASE_TRIG:
			i = in_phase_trig = (STATUS*)o;
			break;
		case inputnames::IN_DEG_SIZE:
			i = in_deg_size = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool sine_wave::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_RECYCLE_MODE:
			set_recycle_mode(*(STATUS*)data);
			retv = true;
			break;
		case paramnames::PAR_TRIG_ZERO_DEG:
			set_on_trig_zero_deg(*(STATUS*)data);
			retv = true;
			break;
		case paramnames::PAR_CYCLES:
			set_cycles(*(double*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

void sine_wave::init()
{
	maxdegs = cycles * 360.00;
}

void sine_wave::run() 
{
	if (*in_phase_trig == ON)
	{
		play_state = ON;
		if (zero_deg == ON)
			degs = 0.00;
	}
	degs += *in_deg_size;
	if (degs >= maxdegs) 
		degs -= maxdegs;
	if (play_state == ON)
	{
		if (degs + *in_deg_size >= maxdegs) 
		{
			if (recycle == OFF)
			{
				play_state = OFF;
			}
		}
		output = sin(degs * DTR);
	}
}

int sine_wave::sine_wave_count = 0;
bool sine_wave::done_params = false;

void sine_wave::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_SINEWAVE, paramnames::PAR_RECYCLE_MODE);
	get_paramlist()->add_param(synthmodnames::MOD_SINEWAVE, paramnames::PAR_TRIG_ZERO_DEG);
	get_paramlist()->add_param(synthmodnames::MOD_SINEWAVE, paramnames::PAR_CYCLES);
	done_params = true;
}

#endif
