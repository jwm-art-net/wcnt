#include "../include/stereoamp.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

stereo_amp::stereo_amp(char const* uname) :
 synthmod(synthmodnames::STEREOAMP, uname, SM_HAS_STEREO_OUTPUT),
 in_left(0), in_right(0), in_amp_eg(0), in_amp_mod(0),
 out_left(0), out_right(0),
 left_level(0), right_level(0), amp_modsize(0), clip_level(0),
 left(0), right(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_LEFT);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_RIGHT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_LEFT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_RIGHT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_EG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_AMP_MOD);
    create_params();
}

stereo_amp::~stereo_amp()
{
}

void const* stereo_amp::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_LEFT:     return &out_left;
        case outputnames::OUT_RIGHT:    return &out_right;
       default: return 0;
    }
}

void const* stereo_amp::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_LEFT:   return in_left = (double*)o;
        case inputnames::IN_RIGHT:  return in_right = (double*)o;
        case inputnames::IN_EG:     return in_amp_eg = (double*)o;
        case inputnames::IN_AMP_MOD:return in_amp_mod = (double*)o;
        default: return 0;
    }
}

void const* stereo_amp::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_LEFT:    return in_left;
        case inputnames::IN_RIGHT:   return in_right;
        case inputnames::IN_EG:      return in_amp_eg;
        case inputnames::IN_AMP_MOD: return in_amp_mod;
        default: return 0;
    }
}

bool stereo_amp::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::LEFT_LEVEL:
            left_level = *(double*)data;
            return true;
        case paramnames::RIGHT_LEVEL:
            right_level = *(double*)data;
            return true;
        case paramnames::AMP_MODSIZE:
            amp_modsize = *(double*)data;
            return true;
        case paramnames::CLIP_LEVEL:
            clip_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* stereo_amp::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::LEFT_LEVEL:  return &left_level;
        case paramnames::RIGHT_LEVEL: return &right_level;
        case paramnames::AMP_MODSIZE: return &amp_modsize;
        case paramnames::CLIP_LEVEL:  return &clip_level;
        default: return 0;
    }
}

stockerrs::ERR_TYPE stereo_amp::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::AMP_MODSIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::AMP_MODSIZE);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::CLIP_LEVEL,
            stockerrs::ERR_NEG_ZERO))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::CLIP_LEVEL);
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    return stockerrs::ERR_NO_ERROR;
}

void stereo_amp::run()
{
    left = *in_left * ((left_level * (1 - amp_modsize) +
           left_level * *in_amp_mod * amp_modsize) * *in_amp_eg);
    out_left = left;
    if (left < -clip_level)
        out_left = -clip_level;
    else if (left > clip_level)
        out_left = clip_level;
    right = *in_right * ((right_level * (1 - amp_modsize) +
            right_level * *in_amp_mod * amp_modsize) * *in_amp_eg);
    out_right = right;
    if (right < -clip_level)
        out_right = -clip_level;
    else if (right > clip_level)
        out_right = clip_level;
}

void stereo_amp::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::LEFT_LEVEL);
    relate_param(paramnames::RIGHT_LEVEL);
    relate_param(paramnames::AMP_MODSIZE);
    relate_param(paramnames::CLIP_LEVEL);
}

