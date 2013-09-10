#ifndef LFOCONTROL_H
#include "../include/lfocontroller.h"

lfo_controller::lfo_controller(string uname) :
	synthmod(synthmodnames::MOD_LFOCONTROL, lfo_controller_count, uname),
	in_trig(0), in_wave(0), in_amp_mod(0), out_preampmod(0), output(0),
 	delay_time(0), ramp_time(0), start_level(0), end_level(0), 
	response_time(0), amp_modsize(0), ams_r(0), delay_samples(0), 
	ramp_samples(0), resp_size(0), resp_fa_level(0), resp_ac(0), 
	level_size(0), current_level(0)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	get_outputlist()->add_output(this, outputnames::OUT_PRE_AMP_MOD);
	get_inputlist()->add_input(this, inputnames::IN_TRIG);
	get_inputlist()->add_input(this, inputnames::IN_WAVE);
	get_inputlist()->add_input(this, inputnames::IN_AMP_MOD);
	#endif
	lfo_controller_count++;
	validate();
	#ifndef BARE_MODULES
	create_params();
	#endif
}

lfo_controller::~lfo_controller() 
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
}

void lfo_controller::init()
{
	resp_size = (double) 1 / convert_ms_to_samples(response_time);
}

#ifndef BARE_MODULES
void const* lfo_controller::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot) {
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
	switch(it) {
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
	switch(pt){
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
		case paramnames::PAR_RESPONSE_TIME:
			set_response_time(*(double*)data);
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
#endif // BARE_MODULES

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
	if (*in_trig == ON)	{
		resp_fa_level = output;
		resp_ac = 1;
		current_level = start_level;
		delay_samples = convert_ms_to_samples(delay_time);
		ramp_samples = convert_ms_to_samples(ramp_time);
		level_size = (end_level - start_level) / ramp_samples;
	} 
	else if (delay_samples > 0) 
		delay_samples--;
	else if (ramp_samples > 0) {
		current_level += level_size;
		ramp_samples--;
	}
	if (resp_ac > 0) {
//		out_preampmod = *in_wave * resp_fa_level * resp_ac 
//			+ *in_wave * current_level * (1 - resp_ac);
		out_preampmod = resp_fa_level * resp_ac 
			+ *in_wave * current_level * (1 - resp_ac);
		resp_ac -= resp_size;
	}
	else
		out_preampmod = *in_wave * current_level;
	if (out_preampmod > 1) 
		out_preampmod = 1; 
	else if (out_preampmod < -1) 
		out_preampmod = -1;
	output = out_preampmod * ams_r + out_preampmod * *in_amp_mod * amp_modsize;
}

int lfo_controller::lfo_controller_count = 0;

#ifndef BARE_MODULES
bool lfo_controller::done_params = false;

void lfo_controller::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_LFOCONTROL, paramnames::PAR_DELAY_TIME);
	get_paramlist()->add_param(synthmodnames::MOD_LFOCONTROL, paramnames::PAR_RAMP_TIME);
	get_paramlist()->add_param(synthmodnames::MOD_LFOCONTROL, paramnames::PAR_START_LEVEL);
	get_paramlist()->add_param(synthmodnames::MOD_LFOCONTROL, paramnames::PAR_END_LEVEL);
	get_paramlist()->add_param(synthmodnames::MOD_LFOCONTROL, paramnames::PAR_RESPONSE_TIME);
	get_paramlist()->add_param(synthmodnames::MOD_LFOCONTROL, paramnames::PAR_AMP_MODSIZE);
	done_params = true;
}
#endif
#endif
