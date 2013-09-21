#include "../include/monoamp.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

mono_amp::mono_amp(char const* uname) :
 synthmod(synthmodnames::MONOAMP, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), in_amp_eg(0), in_amp_mod(0),
 out_output(0),
 level(0), amp_modsize(0), clip_level(0), amp_level(0), ampsig(0)
{
    register_input(inputnames::IN_SIGNAL);
    register_input(inputnames::IN_EG);
    register_input(inputnames::IN_AMP_MOD);
    register_output(outputnames::OUT_OUTPUT);
    init_first();
}

mono_amp::~mono_amp()
{
}

void const* mono_amp::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* mono_amp::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal = (double*)o;
        case inputnames::IN_EG:     return in_amp_eg = (double*)o;
        case inputnames::IN_AMP_MOD:return in_amp_mod = (double*)o;
        default: return 0;
    }
}

void const* mono_amp::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        case inputnames::IN_EG:     return in_amp_eg;
        case inputnames::IN_AMP_MOD:return in_amp_mod;
        default: return 0;
    }
}

bool mono_amp::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::LEVEL:
            level = *(double*)data;
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

void const* mono_amp::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::LEVEL:         return &level;
        case paramnames::AMP_MODSIZE:   return &amp_modsize;
        case paramnames::CLIP_LEVEL:    return &clip_level;
        default: return 0;
    }
}

stockerrs::ERR_TYPE mono_amp::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::AMP_MODSIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg =
         paramnames::get_name(paramnames::AMP_MODSIZE);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::CLIP_LEVEL,
            stockerrs::ERR_NEG_ZERO))
    {
        *err_msg =
         paramnames::get_name(paramnames::CLIP_LEVEL);
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    return stockerrs::ERR_NO_ERROR;
}

void mono_amp::run()
{
    ampsig = (level * (1 - amp_modsize) + level *
              *in_amp_mod * amp_modsize) * *in_signal * *in_amp_eg;
    out_output = ampsig;
    if (ampsig < -clip_level)
        out_output = -clip_level;
    else if (ampsig > clip_level)
        out_output = clip_level;
}

void mono_amp::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::LEVEL);
    register_param(paramnames::AMP_MODSIZE);
    register_param(paramnames::CLIP_LEVEL);
}

