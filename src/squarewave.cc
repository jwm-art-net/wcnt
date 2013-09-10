#ifndef SQUAREWAVE_H
#include "../include/squarewave.h"

square_wave::square_wave(string uname)
:synthmod(synthmodnames::MOD_SQUAREWAVE, square_wave_count, uname),
output(0.00), out_offpulse(OFF), play_state(OFF), in_phase_trig(NULL), in_deg_size(NULL), in_pwm(NULL),
degs(0.00), pulse_width(0.50), pwm_size(0.00), poff_deg(0.00), pulse(OFF), recycle(OFF), cycle(0)
{
	if (!get_outputlist()->add_output(this, outputnames::OUT_OUTPUT)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_OFF_PULSE)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_PHASE_TRIG)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_DEG_SIZE)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_PWM)){
		invalidate();
		return;
	}
	square_wave_count++;
	validate();
	create_params();
}

square_wave::~square_wave() 
{
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
}

void const* square_wave::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_OUTPUT:
			o = &output;
			break;
		case outputnames::OUT_OFF_PULSE:
			o = &out_offpulse;
			break;
		case outputnames::OUT_PLAY_STATE:
			o = &play_state;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* square_wave::set_in(inputnames::IN_TYPE it, void const* o)
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
		case inputnames::IN_PWM:
			i = in_pwm =  (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool square_wave::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_PULSE_WIDTH:
			set_pulse_width(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_PWM_SIZE:
			set_pwm_size(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_RECYCLE_MODE:
			set_recycle_mode(*(STATUS*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

void square_wave::init()
{
}

void square_wave::run() 
{
	if (*in_phase_trig == ON)	
	{
		play_state = ON;
		poff_deg = 360.00 * (pulse_width + pwm_size * *in_pwm);
		if (poff_deg < 1.00)
			poff_deg = 1.00;
		else if (poff_deg > 359.00)
			poff_deg = 359.00;
		output = 1;
		pulse = ON;
	}
	degs += *in_deg_size;
	if (degs >= 360.00) 
		degs -= 360.00;
	if (play_state == ON)
	{
		if (degs + *in_deg_size >= 360.00)
		{
			if (recycle == OFF)
			{
				play_state = OFF;
				output = 0;
			}
			else
			{
				poff_deg = 360.00 * (pulse_width + pwm_size * *in_pwm);
				if (poff_deg < 1.00) 
					poff_deg = 1.00;
				else if (poff_deg > 359.00) 
					poff_deg = 359.00;
				pulse = ON;
			}
		}
		if (pulse == ON)
		{		
			if (degs >= poff_deg)
			{
				output = -1.00;
				pulse = OFF;
				out_offpulse = ON;
			}
				
		}
		else if (out_offpulse == ON)
			out_offpulse = OFF;
	}
}

int square_wave::square_wave_count = 0;
bool square_wave::done_params = false;

void square_wave::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_SQUAREWAVE, paramnames::PAR_PULSE_WIDTH);
	get_paramlist()->add_param(synthmodnames::MOD_SQUAREWAVE, paramnames::PAR_PWM_SIZE);
	get_paramlist()->add_param(synthmodnames::MOD_SQUAREWAVE, paramnames::PAR_RECYCLE_MODE);
	done_params = true;
}

#endif
