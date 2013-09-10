#ifndef TRIANGLEWAVE2_H
#include "../include/trianglewave2.h"

triangle_wave2::triangle_wave2(string uname)
: synthmod(synthmodnames::MOD_TRIWAVE2, triangle_wave2_count, uname),
  in_phase_trig(0), in_deg_size(0), in_normal_mod(0), output(0.00), play_state(OFF),
  normal_freq(440.00), norm_mod_hi(880), norm_mod_lo(220), recycle(OFF), zero_retrigger_mode(OFF),
  nf_deg_size(0), nf_pre_deg_size(0), fmrad(0), fmmid(0), nf_sectmaxsamps(0), 
  nf_sectsample(0), nf_counter_ratio(0.0), sect(0), sect_spanlvl(0), sect_startlvl(0),
  old_maxsamps(0), sectmaxsamps(1), sectsample(0), counter_ratio(0)
{
	if (!get_outputlist()->add_output(this, outputnames::OUT_OUTPUT))
	{
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE))
	{
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_PHASE_TRIG))
	{
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_DEG_SIZE))
	{
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_NORM_MOD))
	{
		invalidate();
		return;
	}
	triangle_wave2_count++;
	validate();
	create_params();
}

triangle_wave2::~triangle_wave2()
{
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
}

void const* triangle_wave2::get_out(outputnames::OUT_TYPE ot)
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

void const* triangle_wave2::set_in(inputnames::IN_TYPE it, void const* o)
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
		case inputnames::IN_NORM_MOD:
			i = in_normal_mod = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool triangle_wave2::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_NORM_FREQ:
			set_normal_frequency(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_NORM_MOD_HI:
			set_normal_mod_range_hi(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_NORM_MOD_LO:
			set_normal_mod_range_lo(*(double*)data);
			retv = true;
			break;
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

void triangle_wave2::init()
{
	nf_pre_deg_size = freq_to_step(normal_freq, 0);
	fmrad = (norm_mod_hi - norm_mod_lo) / 2; 
	fmmid = norm_mod_lo + fmrad;
}
	
void triangle_wave2::run()
{
	if (*in_phase_trig == ON)	
	{
		play_state = ON;
		sect = 0;
		if (zero_retrigger_mode == ON) 
			output = 0;
		sect_startlvl = output;
		sect_spanlvl = 1 - sect_startlvl;
		sectsample = 0;
		nf_sectsample = 0;
	}
	if (play_state == ON)
	{
		nf_deg_size = 	nf_pre_deg_size + nf_pre_deg_size * (fmmid + fmrad * *in_normal_mod);
		nf_counter_ratio = nf_sectsample / nf_sectmaxsamps;
		nf_sectmaxsamps = 90 / nf_deg_size;
		nf_sectsample 	= nf_sectmaxsamps 	* nf_counter_ratio;
		nf_sectsample++;
		if (nf_sectsample < nf_sectmaxsamps)
			output = sect_startlvl + sect_spanlvl * nf_counter_ratio;
		counter_ratio = sectsample / sectmaxsamps;
		sectmaxsamps 	= 90 / *in_deg_size;
		sectsample = sectmaxsamps * counter_ratio;
		sectsample++;
		if (sectsample >= sectmaxsamps)	
		{
			sectsample = 0;
			nf_sectsample = 0;
			sect++;
			switch(sect) 
			{
				case 1:
					sect_startlvl = output;
					sect_spanlvl = -sect_startlvl;
					break;
				case 2:
					sect_startlvl = output;
					sect_spanlvl = sect_startlvl - 1;
					break;
				case 3:
					sect_startlvl = output;
					sect_spanlvl = -sect_startlvl;
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

int triangle_wave2::triangle_wave2_count = 0;
bool triangle_wave2::done_params = false;

void triangle_wave2::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_TRIWAVE2, paramnames::PAR_NORM_FREQ);
	get_paramlist()->add_param(synthmodnames::MOD_TRIWAVE2, paramnames::PAR_NORM_MOD_HI);
	get_paramlist()->add_param(synthmodnames::MOD_TRIWAVE2, paramnames::PAR_NORM_MOD_LO);
	get_paramlist()->add_param(synthmodnames::MOD_TRIWAVE2, paramnames::PAR_RECYCLE_MODE);
	get_paramlist()->add_param(synthmodnames::MOD_TRIWAVE2, paramnames::PAR_ZERO_RETRIGGER);
	done_params = true;
}

#endif
