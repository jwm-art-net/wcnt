#ifndef DELAY_H
#include "../include/delay.h"

delay::delay(string uname) :
	synthmod(synthmodnames::MOD_DELAY, delay_count, uname), 
	in_signal(0), in_gainmod(0), out_output(0), delay_time(0), gain(0), 
	gain_modsize(0), wetdry(0), output(0), filter(0), filterarraymax(0), 
	fpos(0), filtertotal(0), gainamount(0)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
	get_inputlist()->add_input(this, inputnames::IN_GAIN_MOD);
	#endif
	delay_count++;
	#ifndef BARE_MODULES
	create_params();
	#endif
}

delay::~delay() 
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
	if (filter)
		delete [] filter;
}

void delay::init()
{
	filterarraymax = (long)((delay_time * audio_samplerate) / 1000);
	filter = new double[filterarraymax];
	for (long i = 0; i < filterarraymax; i++) filter[i] = 0;
	fpos = filterarraymax - 1;
}

#ifndef BARE_MODULES
void const* delay::get_out(outputnames::OUT_TYPE ot)
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

void const* delay::set_in(inputnames::IN_TYPE it, void const* o)
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
		default:
			i = 0;
	}
	return i;
}

bool delay::set_param(paramnames::PAR_TYPE pt, void const* data)
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

bool delay::validate()
{
	if (delay_time < 0) {
		*err_msg = "\n" 
			+ get_paramnames()->get_name(paramnames::PAR_DELAY_TIME) +
			" less than zero";
		invalidate();
	}
	if (gain > 1.0 || gain < -1.0) {
		*err_msg = "\n" 
			+ get_paramnames()->get_name(paramnames::PAR_GAIN) +
			" out of range (-1.0 to +1.0)";
		invalidate();
	}
	if (gain_modsize < 0.0 || gain_modsize > 1.0) {
		*err_msg += "\n" +
			get_paramnames()->get_name(paramnames::PAR_GAIN_MODSIZE) +
			" out of range (0.0 to +1.0)";
		invalidate();
	}
	if (wetdry < 0.0 || wetdry > 1.0) {
		*err_msg += "\n" +
			get_paramnames()->get_name(paramnames::PAR_WETDRY) +
			" out of range (0.0 to +1.0)";
		invalidate();
	}
	return is_valid();
}

#endif // BARE_MODULES

void delay::run() 
{
	if (*in_gainmod >= 0) 
		gainamount = gain - gain * gain_modsize 
			+ (gain * gain_modsize * *in_gainmod);
	else
		gainamount = gain - gain * gain_modsize 
			+ (gain * gain_modsize * *in_gainmod);
	output = filter[fpos];
	filter[fpos] = *in_signal * gainamount;
	fpos--;
	if (fpos < 0) fpos = filterarraymax - 1;
	out_output = output * wetdry + *in_signal * (1 - wetdry);
}

int delay::delay_count = 0;

#ifndef BARE_MODULES
bool delay::done_params = false;

void delay::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_DELAY, paramnames::PAR_DELAY_TIME);
	get_paramlist()->add_param(synthmodnames::MOD_DELAY, paramnames::PAR_GAIN);
	get_paramlist()->add_param(synthmodnames::MOD_DELAY, paramnames::PAR_GAIN_MODSIZE);
	get_paramlist()->add_param(synthmodnames::MOD_DELAY, paramnames::PAR_WETDRY);
	done_params = true;
}
#endif
#endif
