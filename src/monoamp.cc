#ifndef MONOAMP_H
#include "../include/monoamp.h"

mono_amp::mono_amp(char const* uname) :
 synthmod(synthmodnames::MOD_MONOAMP, monoamp_count,	uname),
 in_signal(0), in_amp_eg(0), in_amp_mod(0), out_mono(0), out_m(0),
 amplitude(0), amp_modsize(0), clip_level(0), amp_level(0), ampsig(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_MONO);
    get_outputlist()->add_output(this, outputnames::OUT_M);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    get_inputlist()->add_input(this, inputnames::IN_EG);
    get_inputlist()->add_input(this, inputnames::IN_AMP_MOD);
    monoamp_count++;
    create_params();
}

mono_amp::~mono_amp()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* mono_amp::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_MONO:
        o = &out_mono;
        break;
    case outputnames::OUT_M:
        o = &out_m;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* mono_amp::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal = (double*)o;
    case inputnames::IN_EG:
        return in_amp_eg = (double*)o;
    case inputnames::IN_AMP_MOD:
        return in_amp_mod = (double*)o;
    default:
        return 0;
    }
}

void const* mono_amp::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal;
    case inputnames::IN_EG:
        return in_amp_eg;
    case inputnames::IN_AMP_MOD:
        return in_amp_mod;
    default:
        return 0;
    }
}

bool mono_amp::set_param(paramnames::PAR_TYPE pt, void const* data)
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

void const* mono_amp::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_AMPLITUDE:
        return &amplitude;
    case paramnames::PAR_AMP_MODSIZE:
        return &amp_modsize;
    case paramnames::PAR_CLIP_LEVEL:
        return &clip_level;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE mono_amp::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_AMPLITUDE,
            stockerrs::ERR_RANGE_AMP))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_AMPLITUDE);
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

void mono_amp::run()
{
    ampsig = (amplitude * (1 - amp_modsize) + amplitude *
              *in_amp_mod * amp_modsize) * *in_signal * *in_amp_eg;
    out_mono = (short) ampsig;
    if (ampsig < -clip_level)
        out_mono = -clip_level;
    else if (ampsig > clip_level)
        out_mono= clip_level;
    out_m = (double) out_mono / 32767;
}

int mono_amp::monoamp_count = 0;

bool mono_amp::done_params = false;

void mono_amp::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_MONOAMP, paramnames::PAR_AMPLITUDE);
    get_paramlist()->add_param(
     synthmodnames::MOD_MONOAMP, paramnames::PAR_AMP_MODSIZE);
    get_paramlist()->add_param(
     synthmodnames::MOD_MONOAMP, paramnames::PAR_CLIP_LEVEL);
    done_params = true;
}

#endif
