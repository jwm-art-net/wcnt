#ifndef STEREOAMP_H
#include "../include/stereoamp.h"

stereo_amp::stereo_amp(string uname)
:synthmod(synthmodnames::MOD_STEREOAMP, stereoamp_count,	uname),
out_left(0), out_right(0) ,in_signal(NULL), in_amp_eg(NULL), in_amp_mod(NULL), in_pan_mod(NULL),
amplitude(0), amp_modsize(0.00), pan(0.00), pan_modsize(0.00), clip_mode(CLIP), clip_level(32767), dc_level(0)
{
	if (!get_outputlist()->add_output(this, outputnames::OUT_LEFT)){
		invalidate();
		return;
	}
	if (!get_outputlist()->add_output(this, outputnames::OUT_RIGHT)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_SIGNAL)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_EG)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_AMP_MOD)){
		invalidate();
		return;
	}
	if (!get_inputlist()->add_input(this, inputnames::IN_PAN_MOD)){
		invalidate();
		return;
	}
	stereoamp_count++;
	validate();
	create_params();
}

stereo_amp::~stereo_amp() 
{
	get_outputlist()->delete_module_outputs(this);
	get_inputlist()->delete_module_inputs(this);
}
void const* stereo_amp::get_out(outputnames::OUT_TYPE ot)
{
	void const* o = 0;
	switch(ot)
	{
		case outputnames::OUT_LEFT:
			o = &out_left;
			break;
		case outputnames::OUT_RIGHT:
			o = &out_right;
			break;
		default:
			o = 0;
	}
	return o;
}

void const* stereo_amp::set_in(inputnames::IN_TYPE it, void const* o)
{
	void const* i = 0;
	switch(it)
	{
		case inputnames::IN_SIGNAL:
			i = in_signal = (double*)o;
			break;
		case inputnames::IN_EG:
			i = in_amp_eg = (double*)o;
			break;
		case inputnames::IN_AMP_MOD:
			i = in_amp_mod = (double*)o;
			break;
		case inputnames::IN_PAN_MOD:
			i = in_pan_mod = (double*)o;
			break;
		default:
			i = 0;
	}
	return i;
}

bool stereo_amp::set_param(paramnames::PAR_TYPE pt, void const* data)
{
	bool retv = false;
	switch(pt)
	{
		case paramnames::PAR_AMPLITUDE:
			set_amplitude(*(short*)data);
			retv = true;
			break;
		case paramnames::PAR_AMP_MODSIZE:
			set_amp_modsize(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_PAN:
			set_pan(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_PAN_MODSIZE:
			set_pan_modsize(*(double*)data);
			retv = true;
			break;
		case paramnames::PAR_CLIP_MODE:
			set_clip_mode(*(CLIP_MODE*)data);
			retv = true;
			break;
		case paramnames::PAR_CLIP_LEVEL:
			set_clip_level(*(short*)data);
			retv = true;
			break;
		case paramnames::PAR_DC_LEVEL:
			set_dc_level(*(short*)data);
			retv = true;
			break;
		default: 
			retv = false;
			break;
	}
	return retv;
}

//	_left = _amp_level * ((1 - _pan_pos) / 2);
//	_right = _amp_level * ((1 + _pan_pos) / 2);

void stereo_amp::run() 
{
	_pan_pos = pan + *in_pan_mod * pan_modsize;
	_amp_level = (amplitude - amplitude * amp_modsize) + amplitude * *in_amp_mod * amp_modsize;
	ampsig = _amp_level * *in_signal * *in_amp_eg;
	_left = dc_level * *in_amp_eg * ((1 - _pan_pos)/2) + ampsig * ((1 - _pan_pos) / 2);
	_right = dc_level * *in_amp_eg * ((1 + _pan_pos)/2) + ampsig * ((1 + _pan_pos) / 2);
	if (_left < -clip_level) 
	{
		if (clip_mode == CLIP) 
			out_left = -clip_level;
		else 
			out_left = (short)(-clip_level - (_left + clip_level));
	} 
	else if (_left > clip_level) 
	{
		if (clip_mode == CLIP) 
			out_left = clip_level;
		else 
			out_left = (short)(clip_level - (_left - clip_level));
	} 
	else out_left = (short)_left;
	if (_right < -clip_level) 
	{
		if (clip_mode == CLIP) 
			out_right = -clip_level;
		else 
			out_right = (short)(-clip_level - (_right + clip_level));
	} 
	else if (_right > clip_level) 
	{
		if (clip_mode == CLIP) 
			out_right = clip_level;
		else 
			out_right = (short)(clip_level - (_right - clip_level));
	} 
	else 
		out_right = (short)_right;
}

int stereo_amp::stereoamp_count = 0;
bool stereo_amp::done_params = false;

void stereo_amp::create_params()
{
	if (done_params == true)
		return;
	get_paramlist()->add_param(synthmodnames::MOD_STEREOAMP, paramnames::PAR_AMPLITUDE);
	get_paramlist()->add_param(synthmodnames::MOD_STEREOAMP, paramnames::PAR_AMP_MODSIZE);
	get_paramlist()->add_param(synthmodnames::MOD_STEREOAMP, paramnames::PAR_PAN);
	get_paramlist()->add_param(synthmodnames::MOD_STEREOAMP, paramnames::PAR_PAN_MODSIZE);
	get_paramlist()->add_param(synthmodnames::MOD_STEREOAMP, paramnames::PAR_CLIP_MODE);
	get_paramlist()->add_param(synthmodnames::MOD_STEREOAMP, paramnames::PAR_CLIP_LEVEL);
	get_paramlist()->add_param(synthmodnames::MOD_STEREOAMP, paramnames::PAR_DC_LEVEL);
	done_params = true;
}

#endif
