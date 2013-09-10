#ifndef LFOCLOCK_H
#include "../include/lfoclock.h"

lfo_clock::lfo_clock(string uname) :
	synthmod(synthmodnames::MOD_LFOCLOCK, lfo_clock_count, uname),
	out_phase_trig(OFF), out_deg_size(0.00), out_premod_deg_size(0.00), 
	hrtz_freq(0.00), in_phase_trig(NULL), in_freq_mod1(NULL), 
	in_freq_mod2(NULL), freq_mod1size(0.00), freq_mod2size(0.00), 
	degs(360.00), degsize1(0.00), degsize2(0.00)
{
	// degs initialised at 360 so immediately triggers if in_phase_trig is off
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_PHASE_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_PREMOD_DEG_SIZE);
	get_outputlist()->add_output(this, outputnames::OUT_DEG_SIZE);
	get_inputlist()->add_input(this, inputnames::IN_PHASE_TRIG);
	get_inputlist()->add_input(this, inputnames::IN_FREQ_MOD1);
	get_inputlist()->add_input(this, inputnames::IN_FREQ_MOD2);
	#endif // BARE_MODULES
	lfo_clock_count++;
	#ifndef BARE_MODULES
	create_params();
	#endif
}

lfo_clock::~lfo_clock() 
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
}

#ifndef BARE_MODULES
void const* lfo_clock::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot) {
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

void const* lfo_clock::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it) {
		case inputnames::IN_PHASE_TRIG:
			i = in_phase_trig = (STATUS*)o;
			break;
		case inputnames::IN_FREQ_MOD1:
			i = in_freq_mod1 = (double*)o;
			break;
		case inputnames::IN_FREQ_MOD2:
			i = in_freq_mod2 = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool lfo_clock::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt) {
		case paramnames::PAR_FREQ_MOD1SIZE:
			set_freq_mod1size(*(double*)data); 
			retv = true;
			break;
		case paramnames::PAR_FREQ_MOD2SIZE:
			set_freq_mod2size(*(double*)data); 
			retv = true;
			break;
		case paramnames::PAR_FREQ:
			set_frequency(*(double*)data); 
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

bool lfo_clock::validate()
{
	if (freq_mod1size < 0 || freq_mod1size > 15) {
		*err_msg = "\n" + 
			get_paramnames()->get_name(paramnames::PAR_FREQ_MOD1SIZE) 
			+ " out of range (0.0 to 15.0)";
		invalidate();
	}
	if (freq_mod2size < 0 || freq_mod2size > 15) {
		*err_msg += "\n" +
			get_paramnames()->get_name(paramnames::PAR_FREQ_MOD2SIZE)
			+ " out of range (0.0 to 15.0)";
		invalidate();
	}
	if (hrtz_freq <= 0 || hrtz_freq >= audio_samplerate / 2) {
		*err_msg += "\n" +
			get_paramnames()->get_name(paramnames::PAR_FREQ) +
			" out of range ( > 0.0 to 1/2 samplerate)";
		invalidate();
	}
	return is_valid();
}

#endif

void lfo_clock::init()
{
}

void lfo_clock::set_frequency(double f) 
{
	hrtz_freq = f;
	out_premod_deg_size = freq_to_step(f, 0);
}

void lfo_clock::run() 
{
// old way -- depreciated
// out_deg_size = 
//	out_premod_deg_size + out_premod_deg_size * 0.5 *
// 	(freq_mod1size * *in_freq_mod1 + freq_mod2size * *in_freq_mod2);
// new way is more intensive, but returns truer results (hopefully!)
	if (*in_freq_mod1 < 0)
		degsize1 = out_premod_deg_size / (1 + freq_mod1size * -*in_freq_mod1);
	else
		degsize1 = out_premod_deg_size * (1 + freq_mod1size * *in_freq_mod1);
	if (*in_freq_mod2 < 0)
		degsize2 = out_premod_deg_size / (1 + freq_mod2size * -*in_freq_mod2);
	else
		degsize2 = out_premod_deg_size * (1 + freq_mod2size * *in_freq_mod2);
	out_deg_size = (degsize1 + degsize2) / 2;
	degs += out_deg_size;
	if (degs >= 360) {
		degs -= 360;
		out_phase_trig = ON;
	} 
	else if (out_phase_trig == ON) 
		out_phase_trig = OFF;
	if (*in_phase_trig == ON) {
		degs = 0;
		out_phase_trig = ON;
	}
}

int lfo_clock::lfo_clock_count = 0;

#ifndef BARE_MODULES
bool lfo_clock::done_params = false;

void lfo_clock::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_LFOCLOCK, paramnames::PAR_FREQ);
	get_paramlist()->add_param(synthmodnames::MOD_LFOCLOCK, paramnames::PAR_FREQ_MOD1SIZE);
	get_paramlist()->add_param(synthmodnames::MOD_LFOCLOCK, paramnames::PAR_FREQ_MOD2SIZE);
	done_params = true;
}
#endif
#endif
