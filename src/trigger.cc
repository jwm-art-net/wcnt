#ifndef TRIGGER_H
#include "../include/trigger.h"

trigger::trigger(string uname) 
: synthmod(synthmodnames::MOD_TRIGGER, trigger_count, uname),
  in_signal(0), out_trig(OFF), delay_time(25), trig_level(0.35), delay_samps(0)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_TRIG);
	get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
	#endif
	trigger_count++;
	validate();
	#ifndef BARE_MODULES
	create_params();
	#endif
}

trigger::~trigger()
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
}

#ifndef BARE_MODULES
void const* trigger::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_TRIG:
			o = &out_trig;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* trigger::set_in(inputnames::IN_TYPE it, void const* o)
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

bool trigger::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_DELAY_TIME:
			set_delay_time(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_TRIGGER_LEVEL:
			set_trigger_level(*(double*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}
#endif

void trigger::run() 
{
	if (delay_samps == 0) 
	{
		if (fabs(*in_signal) >= trig_level) 
		{
			delay_samps = convert_ms_to_samples(delay_time);
			out_trig = ON;
		}
	} 
	else 
	{
		if (out_trig == ON) 
			out_trig = OFF;
		delay_samps--;
	}
}

int trigger::trigger_count = 0;

#ifndef BARE_MODULES
bool trigger::done_params = false;

void trigger::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_TRIGGER, paramnames::PAR_DELAY_TIME);
	get_paramlist()->add_param(synthmodnames::MOD_TRIGGER, paramnames::PAR_TRIGGER_LEVEL);
	done_params = true;
}
#endif
#endif
