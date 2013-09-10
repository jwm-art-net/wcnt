#ifndef LFOCONTROL_H
#include "../include/lfocontroller.h"

lfo_controller::lfo_controller(string uname)
:synthmod(synthmodnames::MOD_LFOCONTROL, lfo_controller_count, uname),
out_preampmod(0.00), output(0.00), in_trig(NULL), in_wave(NULL), in_amp_mod(NULL),
delay_time(0.00), ramp_time(0.00), amp_modsize(0.00), ams_r(0.00), delay_samples(0),
ramp_samples(0), start_level(0.00), end_level(1.00), level_size(0.00), current_level(0.00)
{
	if (!get_outputlist()->add_output(this, outputnames::OUT_OUTPUT))	{
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_PRE_AMP_MOD)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_TRIG)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_WAVE)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_AMP_MOD)){
		invalidate();
		return;
	}
	lfo_controller_count++;
	validate();
	create_params();
}

lfo_controller::~lfo_controller() 
{
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
}
	
void const* lfo_controller::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_OUTPUT:
			o = &output;
			break;
		case outputnames::OUT_PRE_AMP_MOD:
			o = &out_preampmod;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* lfo_controller::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_TRIG:
			i = in_trig = (STATUS*)o;
			break;
		case inputnames::IN_WAVE:
			i = in_wave = (double*)o;
			break;
		case inputnames::IN_AMP_MOD:
			i = in_amp_mod = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool lfo_controller::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_DELAY_TIME:
			set_delay_time(*(double*)data); 
			retv = true;
			break;
		case paramnames::PAR_RAMP_TIME:
			set_ramp_time(*(double*)data); 
			retv = true;
			break;
		case paramnames::PAR_START_LEVEL:
			set_start_level(*(double*)data); 
			retv = true;
			break;
		case paramnames::PAR_END_LEVEL:
			set_end_level(*(double*)data); 
			retv = true;
			break;
		case paramnames::PAR_AMP_MODSIZE:
			set_amp_modsize(*(double*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

void lfo_controller::set_amp_modsize(double ams)	
{
		if (ams >= -1) 
			if (ams <= 1) 
				amp_modsize = ams;
			else 
				amp_modsize = 1;
		else 
			amp_modsize = -1;
		if (amp_modsize >= 0) 
			ams_r = 1 - amp_modsize;
		else 
			ams_r = -1 - amp_modsize;
}
	
void lfo_controller::run() 
{
	if (*in_trig == ON)	
	{
		delay_samples = convert_ms_to_samples(delay_time);
		ramp_samples = convert_ms_to_samples(ramp_time);
		current_level = start_level;
		level_size = (end_level - start_level) / ramp_samples;
	} 
	else if (delay_samples > 0) 
		delay_samples--;
	else if (ramp_samples > 0) 
	{
		current_level += level_size;
		ramp_samples--;
	}
	out_preampmod = *in_wave * current_level;
	if (out_preampmod > 1) 
		out_preampmod = 1; 
	else if (out_preampmod < -1) 
		out_preampmod = -1;
	output = out_preampmod * ams_r + out_preampmod * *in_amp_mod * amp_modsize;
}

int lfo_controller::lfo_controller_count = 0;
bool lfo_controller::done_params = false;

void lfo_controller::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_LFOCONTROL, paramnames::PAR_DELAY_TIME);
	get_paramlist()->add_param(synthmodnames::MOD_LFOCONTROL, paramnames::PAR_RAMP_TIME);
	get_paramlist()->add_param(synthmodnames::MOD_LFOCONTROL, paramnames::PAR_START_LEVEL);
	get_paramlist()->add_param(synthmodnames::MOD_LFOCONTROL, paramnames::PAR_END_LEVEL);
	get_paramlist()->add_param(synthmodnames::MOD_LFOCONTROL, paramnames::PAR_AMP_MODSIZE);
	done_params = true;
}

#endif
