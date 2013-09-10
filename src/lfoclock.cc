#ifndef LFOCLOCK_H
#include "../include/lfoclock.h"

lfo_clock::lfo_clock(char const* uname) :
 synthmod(synthmodnames::MOD_LFOCLOCK, lfo_clock_count, uname),
 out_phase_trig(OFF), out_deg_size(0.00), out_premod_deg_size(0.00),
 hrtz_freq(0.00), in_phase_trig(NULL), in_freq_mod1(NULL),
 in_freq_mod2(NULL), freq_mod1size(0.00), freq_mod2size(0.00),
 degs(360.00), degsize1(0.00), degsize2(0.00)
{
// degs initialised at 360 so immediately triggers if in_phase_trig is off
    get_outputlist()->add_output(this, outputnames::OUT_PHASE_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_PREMOD_DEG_SIZE);
    get_outputlist()->add_output(this, outputnames::OUT_DEG_SIZE);
    get_inputlist()->add_input(this, inputnames::IN_PHASE_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_FREQ_MOD1);
    get_inputlist()->add_input(this, inputnames::IN_FREQ_MOD2);
    lfo_clock_count++;
    create_params();
}

lfo_clock::~lfo_clock()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* lfo_clock::get_out(outputnames::OUT_TYPE ot)
{
    switch(ot) {
    case outputnames::OUT_PHASE_TRIG:
        return &out_phase_trig;
        break;
    case outputnames::OUT_PREMOD_DEG_SIZE:
        return &out_premod_deg_size;
        break;
    case outputnames::OUT_DEG_SIZE:
        return &out_deg_size;
        break;
    default:
        return 0;
    }
}

void const* lfo_clock::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it) {
    case inputnames::IN_PHASE_TRIG:
        return in_phase_trig = (STATUS*)o;
    case inputnames::IN_FREQ_MOD1:
        return in_freq_mod1 = (double*)o;
    case inputnames::IN_FREQ_MOD2:
        return in_freq_mod2 = (double*)o;
    default:
        return 0;
    }
}

void const* lfo_clock::get_in(inputnames::IN_TYPE it)
{
    switch(it) {
    case inputnames::IN_PHASE_TRIG:
        return in_phase_trig;
    case inputnames::IN_FREQ_MOD1:
        return in_freq_mod1;
    case inputnames::IN_FREQ_MOD2:
        return in_freq_mod2;
    default:
        return 0;
    }
}

bool lfo_clock::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt) {
    case paramnames::PAR_FREQ_MOD1SIZE:
        set_freq_mod1size(*(double*)data);
        return true;
    case paramnames::PAR_FREQ_MOD2SIZE:
        set_freq_mod2size(*(double*)data);
        return true;
    case paramnames::PAR_FREQ:
        set_frequency(*(double*)data);
        return true;
    default:
        return false;
    }
}

void const* lfo_clock::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_FREQ_MOD1SIZE:
        return &freq_mod1size;
    case paramnames::PAR_FREQ_MOD2SIZE:
        return &freq_mod2size;
    case paramnames::PAR_FREQ:
        return &hrtz_freq;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE lfo_clock::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_FREQ,
            stockerrs::ERR_RANGE_FREQ))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_FREQ);
        invalidate();
        return stockerrs::ERR_RANGE_FREQ;
    }
    if (!pl->validate(this, paramnames::PAR_FREQ_MOD1SIZE,
            stockerrs::ERR_RANGE_FMOD))
    {
        *err_msg
         = get_paramnames()->get_name(paramnames::PAR_FREQ_MOD1SIZE);
        invalidate();
        return stockerrs::ERR_RANGE_FMOD;
    }
    if (!pl->validate(this, paramnames::PAR_FREQ_MOD2SIZE,
            stockerrs::ERR_RANGE_FMOD))
    {
        *err_msg
         = get_paramnames()->get_name(paramnames::PAR_FREQ_MOD2SIZE);
        invalidate();
        return stockerrs::ERR_RANGE_FMOD;
    }
    return stockerrs::ERR_NO_ERROR;
}

void lfo_clock::init()
{
    mod1size = freq_mod1size - 1;
    mod2size = freq_mod2size - 1;
}

void lfo_clock::set_frequency(double f)
{
    hrtz_freq = f;
    out_premod_deg_size = freq_to_step(f, 0);
}

void lfo_clock::run()
{
    if (mod1size == 0)
        degsize1 = out_premod_deg_size;
    else {
        if (*in_freq_mod1 < 0)
            degsize1 = out_premod_deg_size
             / (1 + mod1size * -*in_freq_mod1);
        else
            degsize1 = out_premod_deg_size
             * (1 + mod1size * *in_freq_mod1);
    }
    if (mod2size == 0)
        degsize2 = degsize1;
    else {
        if (*in_freq_mod2 < 0)
            degsize2 = degsize1 / (1 + mod2size * -*in_freq_mod2);
        else
            degsize2 = degsize1 * (1 + mod2size * *in_freq_mod2);
    }
    out_deg_size = degsize2;
    degs += out_deg_size;
    if (degs >= 360) {
        degs -= 360;
        out_phase_trig = ON;
    }
    else if (out_phase_trig == ON)
        out_phase_trig = OFF;
    if (*in_phase_trig == ON) {
        degs = 0;
        out_phase_trig = ON;
    }
}

int lfo_clock::lfo_clock_count = 0;

bool lfo_clock::done_params = false;

void lfo_clock::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_LFOCLOCK, paramnames::PAR_FREQ);
    get_paramlist()->add_param(
     synthmodnames::MOD_LFOCLOCK, paramnames::PAR_FREQ_MOD1SIZE);
    get_paramlist()->add_param(
     synthmodnames::MOD_LFOCLOCK, paramnames::PAR_FREQ_MOD2SIZE);
    done_params = true;
}

#endif
