#include "../include/monoamp.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

mono_amp::mono_amp(const char* uname) :
 synthmod(module::MONOAMP, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), in_amp_eg(0), in_amp_mod(0),
 out_output(0),
 level(0), amp_modsize(0), clip_level(0), amp_level(0), ampsig(0)
{
    register_input(input::IN_SIGNAL);
    register_input(input::IN_EG);
    register_input(input::IN_AMP_MOD);
    register_output(output::OUT_OUTPUT);
    init_first();
}

mono_amp::~mono_amp()
{
}

const void* mono_amp::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* mono_amp::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal = (double*)o;
        case input::IN_EG:     return in_amp_eg = (double*)o;
        case input::IN_AMP_MOD:return in_amp_mod = (double*)o;
        default: return 0;
    }
}

const void* mono_amp::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
        case input::IN_EG:     return in_amp_eg;
        case input::IN_AMP_MOD:return in_amp_mod;
        default: return 0;
    }
}

bool mono_amp::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::LEVEL:
            level = *(double*)data;
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

const void* mono_amp::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::LEVEL:         return &level;
        case param::AMP_MODSIZE:   return &amp_modsize;
        case param::CLIP_LEVEL:    return &clip_level;
        default: return 0;
    }
}

stockerrs::ERR_TYPE mono_amp::validate()
{
    if (!jwm.get_paramlist()->validate(this, param::AMP_MODSIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        sm_err("%s",
                param::names::get(param::AMP_MODSIZE));
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!jwm.get_paramlist()->validate(this, param::CLIP_LEVEL,
            stockerrs::ERR_NEG_ZERO))
    {
        sm_err("%s",
                param::names::get(param::CLIP_LEVEL));
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
    register_param(param::LEVEL);
    register_param(param::AMP_MODSIZE);
    register_param(param::CLIP_LEVEL);
}

