#ifndef CONTRASTER_H
#include "../include/contraster.h"

contraster::contraster(string uname) :
	synthmod(synthmodnames::MOD_CONTRASTER, contraster_count,	uname),
	out_output(0.0), in_signal(0), in_power_mod(0), 
	power_min(1.0), power_max(2.0), rude_mode(OFF), 
	power(0.0), powerrad(0.0)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
	get_inputlist()->add_input(this, inputnames::IN_POWER_MOD);
	get_inputlist()->add_input(this, inputnames::IN_RUDE_SWITCH_TRIG);
	#endif
	contraster_count++;
	#ifndef BARE_MODULES
	create_params();
	#endif
}

contraster::~contraster() 
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
}

#ifndef BARE_MODULES
void const* contraster::get_out(outputnames::OUT_TYPE ot)
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

void const* contraster::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_SIGNAL:
			i = in_signal = (double*)o;
			break;
		case inputnames::IN_POWER_MOD:
			i = in_power_mod = (double*)o;
			break;
	case inputnames::IN_RUDE_SWITCH_TRIG:
	    i = in_rude_switch_trig = (STATUS*)o;
            break;
		default:
			i = 0;
	}
	return i;
}

bool contraster::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_POWER_MIN:
			set_power_min(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_POWER_MAX:
			set_power_max(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_RUDE_MODE:
			set_rude_mode(*(STATUS*)data);
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

bool contraster::validate()
{
	return is_valid();
}

#endif

void contraster::init()
{
	powerrad = (power_max - power_min) / 2;
}

void contraster::run() 
{
	power_mod = (*in_power_mod < 0) ? -*in_power_mod : *in_power_mod;
	power = power_min + powerrad * power_mod;
	output = *in_signal * power;
	if (*in_rude_switch_trig) {
		if (rude_mode)
			rude_mode = OFF;
		else
			rude_mode = ON;
	}
	if (rude_mode) {
		while (output > 1.0 || output < -1.0) {
			if (output > 1.0)
				output = output - 1.0;
			if (output < -1.0)
				output = output + 1.0;
		}
	}
	else {
		while (output > 1.0 || output < -1.0) {
			if (output > 1.0)
				output = 1.0 - (output - 1);
			if (output < -1.0)
				output = -1.0 - (output + 1);
		}
	}
	out_output = output * wetdry + *in_signal * (1.0 - wetdry);
}

int contraster::contraster_count = 0;

#ifndef BARE_MODULES
bool contraster::done_params = false;

void contraster::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_CONTRASTER, paramnames::PAR_POWER_MIN);
	get_paramlist()->add_param(synthmodnames::MOD_CONTRASTER, paramnames::PAR_POWER_MAX);
	get_paramlist()->add_param(synthmodnames::MOD_CONTRASTER, paramnames::PAR_RUDE_MODE);
	get_paramlist()->add_param(synthmodnames::MOD_CONTRASTER, paramnames::PAR_WETDRY);
	done_params = true;
}
#endif
#endif
