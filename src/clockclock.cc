#ifndef CLOCKCLOCK_H
#include "../include/clockclock.h"

clockclock::clockclock(string uname) :
	synthmod(synthmodnames::MOD_CLOCK, clockclock_count, uname),
	out_phase_trig(OFF), out_premod_deg_size(0.00), out_deg_size(0.00), 
	in_freq_mod1(0), note_length_freq(0), hrtz_freq(0.00), 
	freq_mod1size(0.00), mod1size(0.00), degs(360.00)
{
	// degs initialised to 360 so that it immediately triggers if in_phase_trig is off
	#ifndef BARE_MODULES
	get_inputlist()->add_input(this, inputnames::IN_FREQ_MOD1);
	get_outputlist()->add_output(this, outputnames::OUT_PHASE_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_PREMOD_DEG_SIZE);
	get_outputlist()->add_output(this, outputnames::OUT_DEG_SIZE);
	#endif
	clockclock_count++;
	validate();
	#ifndef BARE_MODULES
	create_params();
	#endif
}

clockclock::~clockclock() 
{
	#ifndef BARE_MODULES
	get_inputlist()->delete_module_inputs(this);
	get_outputlist()->delete_module_outputs(this);
	#endif
}

#ifndef BARE_MODULES
void const* clockclock::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_PHASE_TRIG:
			o = &out_phase_trig;
			break;
		case outputnames::OUT_PREMOD_DEG_SIZE:
			o = &out_premod_deg_size;
			break;
		case outputnames::OUT_DEG_SIZE:
			o = &out_deg_size;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* clockclock::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_FREQ_MOD1:
			i = in_freq_mod1 = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool clockclock::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_NOTELEN_FREQ:
			set_notelength_frequency(*(short*)data); 
			retv = true;
			break;
		case paramnames::PAR_FREQ:
			set_frequency(*(double*)data); 
			retv = true;
			break;
		case paramnames::PAR_FREQ_MOD1SIZE:
			set_freq_mod1size(*(double*)data); 
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

#endif // BARE_MODULES

void clockclock::set_notelength_frequency(short nl)
{
	note_length_freq = nl;
	out_premod_deg_size = freq_to_step(notelength_to_frequency(nl), 0);
	hrtz_freq = -1;
}

void clockclock::set_frequency(double f) 
{
	hrtz_freq = f;
	out_premod_deg_size = freq_to_step(f, 0);
	note_length_freq = -1;
}
void clockclock::init()
{  // saves doing this for every sample
	mod1size = freq_mod1size - 1;
}

void clockclock::run() 
{
	if (*in_freq_mod1 < 0) 
		out_deg_size = out_premod_deg_size / (1 + mod1size * -*in_freq_mod1); 
	else
		out_deg_size = out_premod_deg_size * (1 + mod1size * *in_freq_mod1);
	degs += out_deg_size;
	if (degs >= 360) {
		degs -= 360;
		out_phase_trig = ON;
	} 
	else if (out_phase_trig == ON) out_phase_trig = OFF;
}

int clockclock::clockclock_count = 0;

#ifndef BARE_MODULES
bool clockclock::done_params = false;

// "all the ducks are swimming in the water, round and round and round and,
//  round and round and round and" (repeat till insanity beckons)
// - lemonjelly 2003

void clockclock::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_CLOCK, paramnames::PAR_FREQ);
	get_paramlist()->add_param(synthmodnames::MOD_CLOCK, paramnames::PAR_FREQ_MOD1SIZE);
	done_params = true;
}
#endif
#endif
