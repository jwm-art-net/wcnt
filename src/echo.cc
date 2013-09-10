#ifndef ECHO_H
#include "../include/echo.h"

echo::echo(string uname) :
	synthmod(synthmodnames::MOD_ECHO, echo_count, uname), 
	in_signal(0), in_gainmod(0), in_feedback(0), in_feed_mod(0), output(0), 
	wet_output(0), delay_time(0), gain(0), gain_modsize(0), feed_level(0), 
	feed_modsize(0), wetdry(0), filter(0), filterarraymax(0), fpos(0), 
	filtertotal(0), gainamount(0), feedamount(0)
{
	#ifndef BARE_MODULES
	get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
	get_inputlist()->add_input(this, inputnames::IN_GAIN_MOD);
	get_inputlist()->add_input(this, inputnames::IN_FEEDBACK);
	get_inputlist()->add_input(this, inputnames::IN_FB_MOD);
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	get_outputlist()->add_output(this, outputnames::OUT_WET_OUTPUT);
	#endif
	echo_count++;
	validate();
	#ifndef BARE_MODULES
	create_params();
	#endif
}

echo::~echo() 
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
	delete [] filter;
}

void echo::init()
{
	filterarraymax = (short)((delay_time * audio_samplerate) / 1000);
	filter = new double[filterarraymax];
	for (int i = 0; i < filterarraymax; i++) filter[i] = 0;
	fpos = filterarraymax - 1;
}

#ifndef BARE_MODULES
void const* echo::get_out(outputnames::OUT_TYPE ot)
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

void const* echo::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_SIGNAL:
			i = in_signal = (double*)o;
			break;
		case inputnames::IN_GAIN_MOD:
			i = in_gainmod = (double*)o;
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

bool echo::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_DELAY_TIME:
			set_delay_time(*(double*)data); 
			retv = true;
			break;
		case paramnames::PAR_GAIN:
			set_gain_level(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_GAIN_MODSIZE:
			set_gain_modsize(*(double*)data); 
			retv = true;
			break;
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

void echo::run() 
{
	gainamount = gain * (1 - gain_modsize)
		+ (gain * gain_modsize * *in_gainmod);
	feedamount = feed_level * (1 - feed_modsize)
			+ (feed_level * feed_modsize * *in_feed_mod);
	output = *in_signal * gainamount * (1 - wetdry) + 
		(wet_output = filter[fpos]) * wetdry;
	filter[fpos] = *in_signal * gainamount + *in_feedback * feedamount;
	fpos--;
	if (fpos < 0) fpos = filterarraymax - 1;
}

int echo::echo_count = 0;

#ifndef BARE_MODULES
bool echo::done_params = false;

void echo::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_ECHO, paramnames::PAR_DELAY_TIME);
	get_paramlist()->add_param(synthmodnames::MOD_ECHO, paramnames::PAR_GAIN);
	get_paramlist()->add_param(synthmodnames::MOD_ECHO, paramnames::PAR_GAIN_MODSIZE);
	get_paramlist()->add_param(synthmodnames::MOD_ECHO, paramnames::PAR_FEED_LEVEL);
	get_paramlist()->add_param(synthmodnames::MOD_ECHO, paramnames::PAR_FEED_MODSIZE);
	get_paramlist()->add_param(synthmodnames::MOD_ECHO, paramnames::PAR_WETDRY);
	done_params = true;
}
#endif
#endif
