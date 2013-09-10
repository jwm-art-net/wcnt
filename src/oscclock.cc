#ifndef OSCCLOCK_H
#include "../include/oscclock.h"

osc_clock::osc_clock(string uname) : 
	synthmod(synthmodnames::MOD_OSCCLOCK, osc_clock_count, uname),
	out_phase_trig(OFF), out_deg_size(0.00), out_premod_deg_size(0.00),
	in_freq(NULL), in_note_on_trig(NULL), in_note_slide_trig(NULL), 
	in_freq_mod1(NULL), in_freq_mod2(NULL), octave_offset(0), 
	freq_mod1size(0.00), freq_mod2size(0.00), mod1size(0.00), mod2size(0.00),
	degs(360.00), degsize1(0.00), degsize2(0.00), portamento(5), 
	slide_size(0.00), target_deg_size(0.00), slidesamples(0)
{
	//degs initialised with 360 so that it immediately triggers if in_phase_trig is off
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_PHASE_TRIG);
	get_outputlist()->add_output(this, outputnames::OUT_PREMOD_DEG_SIZE);
	get_outputlist()->add_output(this, outputnames::OUT_DEG_SIZE);
	get_inputlist()->add_input(this, inputnames::IN_NOTE_ON_TRIG);
	get_inputlist()->add_input(this, inputnames::IN_NOTE_SLIDE_TRIG);
	get_inputlist()->add_input(this, inputnames::IN_FREQ);
	get_inputlist()->add_input(this, inputnames::IN_FREQ_MOD1);
	get_inputlist()->add_input(this, inputnames::IN_FREQ_MOD2);
	#endif
	osc_clock_count++;
	validate();
	#ifndef BARE_MODULES
	create_params();
	#endif
}

osc_clock::~osc_clock() 
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
}

void osc_clock::set_tuning_semitones(double s)
{
	if (s < -12)
		semitones = -12;
	else if (s > 12)
		semitones = 12;
	else 
		semitones = s;
}

#ifndef BARE_MODULES
void const* osc_clock::get_out(outputnames::OUT_TYPE ot)
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

void const* osc_clock::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_NOTE_ON_TRIG:
			i = in_note_on_trig = (STATUS*)o;
			break;
		case inputnames::IN_NOTE_SLIDE_TRIG:
			i = in_note_slide_trig = (STATUS*)o;
			break;
		case inputnames::IN_FREQ:
			i = in_freq = (double*)o;
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

bool osc_clock::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_OCTAVE:
			set_octave(*(char*)data);
			retv = true;
			break;
		case paramnames::PAR_TUNING_SEMITONES:
			set_tuning_semitones(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_FREQ_MOD1SIZE:
			set_freq_mod1size(*(double*)data); 
			retv = true;
			break;
		case paramnames::PAR_FREQ_MOD2SIZE:
			set_freq_mod2size(*(double*)data); 
			retv = true;
			break;
		case paramnames::PAR_PORTAMENTO:
			set_portamento(*(double*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}
#endif // BARE_MODULES

void osc_clock::init()
{
	mod1size = freq_mod1size - 1;
	mod2size = freq_mod2size - 1;
}

void osc_clock::run() 
{
	if (*in_note_slide_trig == ON)
	{
		target_deg_size = freq_to_step(*in_freq, octave_offset, semitones);
		slidesamples = convert_ms_to_samples(portamento);
		slide_size = (double)(target_deg_size - out_premod_deg_size) / slidesamples;
	} 
	else if (*in_note_on_trig == ON) 
		out_premod_deg_size = freq_to_step(*in_freq, octave_offset, semitones);
	if (slidesamples > 0) 
	{
		out_premod_deg_size += slide_size;
		slidesamples--;
	}
//	out_deg_size = out_premod_deg_size + out_premod_deg_size * (fm1mid + fm1rad * *in_freq_mod1 + fm2mid + fm2rad * *in_freq_mod2 );
	if (*in_freq_mod1 < 0)
		degsize1 = out_premod_deg_size / (1 + mod1size * -*in_freq_mod1);
	else
		degsize1 = out_premod_deg_size * (1 + mod1size * *in_freq_mod1);
	if (*in_freq_mod2 < 0)
		degsize2 = out_premod_deg_size / (1 + mod2size * -*in_freq_mod2);
	else
		degsize2 = out_premod_deg_size * (1 + mod2size * *in_freq_mod2);
	out_deg_size = (degsize1 + degsize2) / 2;
	degs += out_deg_size;
	if (degs > 360.00) 
	{
		degs -= 360.00;
		out_phase_trig = ON;
	}
	else if (out_phase_trig == ON) 
		out_phase_trig = OFF;
}

int osc_clock::osc_clock_count = 0;

#ifndef BARE_MODULES
bool osc_clock::done_params = false;

void osc_clock::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_OSCCLOCK, paramnames::PAR_OCTAVE);
	get_paramlist()->add_param(synthmodnames::MOD_OSCCLOCK, paramnames::PAR_TUNING_SEMITONES);
	get_paramlist()->add_param(synthmodnames::MOD_OSCCLOCK, paramnames::PAR_PORTAMENTO);
	get_paramlist()->add_param(synthmodnames::MOD_OSCCLOCK, paramnames::PAR_FREQ_MOD1SIZE);
	get_paramlist()->add_param(synthmodnames::MOD_OSCCLOCK, paramnames::PAR_FREQ_MOD2SIZE);
	done_params = true;
}
#endif
#endif
