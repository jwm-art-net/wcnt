#ifndef STEREOAMP_H
#include "../include/stereoamp.h"

stereo_amp::stereo_amp(char const* uname) :
 synthmod(synthmodnames::MOD_STEREOAMP, stereoamp_count,	uname),
 in_left(0), in_right(0), in_amp_eg(0), in_amp_mod(0),
 out_left(0), out_right(0), left_amplitude(0), right_amplitude(0),
 amp_modsize(0), clip_level(0), left(0), right(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_LEFT);
    get_outputlist()->add_output(this, outputnames::OUT_RIGHT);
    get_inputlist()->add_input(this, inputnames::IN_L);
    get_inputlist()->add_input(this, inputnames::IN_R);
    get_inputlist()->add_input(this, inputnames::IN_EG);
    get_inputlist()->add_input(this, inputnames::IN_AMP_MOD);
    stereoamp_count++;
    create_params();
}

stereo_amp::~stereo_amp()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* stereo_amp::get_out(outputnames::OUT_TYPE ot)
{
    switch(ot)
    {
    case outputnames::OUT_LEFT:
        return &out_left;
    case outputnames::OUT_RIGHT:
        return &out_right;
    default:
        return 0;
    }
}

void const* stereo_amp::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_L:
        return in_left = (double*)o;
    case inputnames::IN_R:
        return in_right = (double*)o;
    case inputnames::IN_EG:
        return in_amp_eg = (double*)o;
    case inputnames::IN_AMP_MOD:
        return in_amp_mod = (double*)o;
    default:
        return 0;
    }
}

void const* stereo_amp::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_L:
        return in_left;
    case inputnames::IN_R:
        return in_right;
    case inputnames::IN_EG:
        return in_amp_eg;
    case inputnames::IN_AMP_MOD:
        return in_amp_mod;
    default:
        return 0;
    }
}

bool stereo_amp::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_LEFT_AMPLITUDE:
        set_left_amplitude(*(short*)data);
        retv = true;
        break;
    case paramnames::PAR_RIGHT_AMPLITUDE:
        set_right_amplitude(*(short*)data);
        retv = true;
        break;
    case paramnames::PAR_AMP_MODSIZE:
        set_amp_modsize(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_CLIP_LEVEL:
        set_clip_level(*(short*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* stereo_amp::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_LEFT_AMPLITUDE:
        return &left_amplitude;
    case paramnames::PAR_RIGHT_AMPLITUDE:
        return &right_amplitude;
    case paramnames::PAR_AMP_MODSIZE:
        return &amp_modsize;
    case paramnames::PAR_CLIP_LEVEL:
        return &clip_level;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE stereo_amp::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_LEFT_AMPLITUDE,
            stockerrs::ERR_RANGE_AMP))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_LEFT_AMPLITUDE);
        invalidate();
        return stockerrs::ERR_RANGE_AMP;
    }
    if (!pl->validate(this, paramnames::PAR_RIGHT_AMPLITUDE,
            stockerrs::ERR_RANGE_AMP))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_RIGHT_AMPLITUDE);
        invalidate();
        return stockerrs::ERR_RANGE_AMP;
    }
    if (!pl->validate(this, paramnames::PAR_AMP_MODSIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_AMP_MODSIZE);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!pl->validate(this, paramnames::PAR_CLIP_LEVEL,
            stockerrs::ERR_RANGE_CLIP))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_CLIP_LEVEL);
        invalidate();
        return stockerrs::ERR_RANGE_CLIP;
    }
    return stockerrs::ERR_NO_ERROR;
}

void stereo_amp::run()
{
    left = *in_left * ((left_amplitude * (1 - amp_modsize)
     + left_amplitude * *in_amp_mod * amp_modsize) * *in_amp_eg);
    out_left = (short) left;
    if (left < -clip_level)
        out_left = -clip_level;
    else if (left > clip_level)
        out_left = clip_level;
    right = *in_right * ((right_amplitude * (1 - amp_modsize)
     + right_amplitude * *in_amp_mod * amp_modsize) * *in_amp_eg);
    out_right = (short) right;
    if (right < -clip_level)
        out_right = -clip_level;
    else if (right > clip_level)
        out_right = clip_level;
}

int stereo_amp::stereoamp_count = 0;

bool stereo_amp::done_params = false;

void stereo_amp::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_STEREOAMP, paramnames::PAR_LEFT_AMPLITUDE);
    get_paramlist()->add_param(
     synthmodnames::MOD_STEREOAMP, paramnames::PAR_RIGHT_AMPLITUDE);
    get_paramlist()->add_param(
     synthmodnames::MOD_STEREOAMP, paramnames::PAR_AMP_MODSIZE);
    get_paramlist()->add_param(
     synthmodnames::MOD_STEREOAMP, paramnames::PAR_CLIP_LEVEL);
    done_params = true;
}

#endif
