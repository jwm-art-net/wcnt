#ifndef TRIANGLEWAVE_H
#include "../include/trianglewave.h"

triangle_wave::triangle_wave(string uname)
: synthmod(synthmodnames::MOD_TRIWAVE, triangle_wave_count, uname),
  in_phase_trig(0), in_deg_size(0), output(0), play_state(OFF), sect(0), sect_spanlvl(0), sect_startlvl(0),
  sectmaxsamps(1), sectsample(0), counter_ratio(0), recycle(OFF), zero_retrigger_mode(OFF)
{
	#ifndef BARE_MODULES
	get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
	get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE);
	get_inputlist()->add_input(this, inputnames::IN_PHASE_TRIG);
	get_inputlist()->add_input(this, inputnames::IN_DEG_SIZE);
	#endif
	triangle_wave_count++;
	validate();
	#ifndef BARE_MODULES
	create_params();
	#endif
}

triangle_wave::~triangle_wave()
{
	#ifndef BARE_MODULES
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
	#endif
}

#ifndef BARE_MODULES
void const* triangle_wave::get_out(outputnames::OUT_TYPE ot)
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

void const* triangle_wave::set_in(inputnames::IN_TYPE it, void const* o)
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

bool triangle_wave::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_RECYCLE_MODE:
			set_recycle_mode(*(STATUS*)data);
			retv = true;
			break;
		case paramnames::PAR_ZERO_RETRIGGER:
			set_zero_retrigger_mode(*(STATUS*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}
#endif

void triangle_wave::run() 
{
	if (*in_phase_trig == ON)
	{
		play_state = ON;
		sect = 0;
		sectsample = 0;
		if (zero_retrigger_mode == ON) 
			output = 0;
		sect_startlvl = output;
		sect_spanlvl = 1 - sect_startlvl;
	}
	if (play_state == ON)
	{
		counter_ratio = (double) sectsample / sectmaxsamps;
		sectmaxsamps = 90 / *in_deg_size; 
		sectsample = sectmaxsamps * counter_ratio;
		output = sect_startlvl + sect_spanlvl * counter_ratio;
		sectsample++;
		if (sectsample >= sectmaxsamps)	
		{
			sectsample = 0;
			sect++;
			switch(sect) 
			{
				case 1:
					sect_startlvl = 1;
					sect_spanlvl = -1;
					break;
				case 2:
					sect_startlvl = 0;
					sect_spanlvl = -1;
					break;
				case 3:
					sect_startlvl = -1;
					sect_spanlvl = 1;
					break;
				default:				
					if (recycle == ON) 
					{
						sect = 0;
						if (zero_retrigger_mode == ON) 
							output = 0;
						sect_startlvl = output;
						sect_spanlvl = 1 - sect_startlvl;
					} 	
					else 
						play_state = OFF;
			}
		}
	}
}

short triangle_wave::triangle_wave_count = 0;

#ifndef BARE_MODULES
bool triangle_wave::done_params = false;

void triangle_wave::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_TRIWAVE, paramnames::PAR_RECYCLE_MODE);
	get_paramlist()->add_param(synthmodnames::MOD_TRIWAVE, paramnames::PAR_ZERO_RETRIGGER);
	done_params = true;
}
#endif
#endif
