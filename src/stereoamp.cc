#include "../include/stereoamp.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

stereo_amp::stereo_amp(const char* uname) :
 synthmod(module::STEREOAMP, uname, SM_HAS_STEREO_OUTPUT),
 in_left(0), in_right(0), in_amp_eg(0), in_amp_mod(0),
 out_left(0), out_right(0),
 left_level(0), right_level(0), amp_modsize(0), clip_level(0),
 left(0), right(0)
{
    register_input(input::IN_LEFT);
    register_input(input::IN_RIGHT);
    register_input(input::IN_EG);
    register_input(input::IN_AMP_MOD);
    register_output(output::OUT_LEFT);
    register_output(output::OUT_RIGHT);
    init_first();
}

stereo_amp::~stereo_amp()
{
}

const void* stereo_amp::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_LEFT:     return &out_left;
        case output::OUT_RIGHT:    return &out_right;
       default: return 0;
    }
}

const void* stereo_amp::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_LEFT:   return in_left = (double*)o;
        case input::IN_RIGHT:  return in_right = (double*)o;
        case input::IN_EG:     return in_amp_eg = (double*)o;
        case input::IN_AMP_MOD:return in_amp_mod = (double*)o;
        default: return 0;
    }
}

const void* stereo_amp::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_LEFT:    return in_left;
        case input::IN_RIGHT:   return in_right;
        case input::IN_EG:      return in_amp_eg;
        case input::IN_AMP_MOD: return in_amp_mod;
        default: return 0;
    }
}

bool stereo_amp::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::LEFT_LEVEL:
            left_level = *(double*)data;
            return true;
        case param::RIGHT_LEVEL:
            right_level = *(double*)data;
            return true;
        case param::AMP_MODSIZE:
            amp_modsize = *(double*)data;
            return true;
        case param::CLIP_LEVEL:
            clip_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* stereo_amp::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::LEFT_LEVEL:  return &left_level;
        case param::RIGHT_LEVEL: return &right_level;
        case param::AMP_MODSIZE: return &amp_modsize;
        case param::CLIP_LEVEL:  return &clip_level;
        default: return 0;
    }
}

stockerrs::ERR_TYPE stereo_amp::validate()
{
    if (!jwm.get_paramlist()->validate(this, param::AMP_MODSIZE,
                                            stockerrs::ERR_RANGE_0_1))
    {
        sm_err("%s", param::names::get(
                                            param::AMP_MODSIZE));
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!jwm.get_paramlist()->validate(this, param::CLIP_LEVEL,
                                            stockerrs::ERR_NEG_ZERO))
    {
        sm_err("%s", param::names::get(
                                            param::CLIP_LEVEL));
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

void stereo_amp::init_first()
{
    if (done_first())
        return;
    register_param(param::LEFT_LEVEL);
    register_param(param::RIGHT_LEVEL);
    register_param(param::AMP_MODSIZE);
    register_param(param::CLIP_LEVEL);
}

