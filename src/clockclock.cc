#ifndef CLOCKCLOCK_H
#include "../include/clockclock.h"

clockclock::clockclock(char const* uname) :
 synthmod(synthmodnames::MOD_CLOCK, clockclock_count, uname),
 out_phase_trig(OFF), out_premod_deg_size(0.00), out_deg_size(0.00),
 in_freq_mod1(0), hrtz_freq(0.00), freq_mod1size(0.00), degs(360.00)
{
    // degs initialised to 360 so that it immediately triggers
#ifndef BARE_MODULES
    get_inputlist()->add_input(this, inputnames::IN_FREQ_MOD1);
    get_outputlist()->add_output(this, outputnames::OUT_PHASE_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_PREMOD_DEG_SIZE);
    get_outputlist()->add_output(this, outputnames::OUT_DEG_SIZE);
#endif
    clockclock_count++;
#ifndef BARE_MODULES
    create_params();
#endif
}

clockclock::~clockclock()
{
#ifndef BARE_MODULES
    get_inputlist()->delete_module_inputs(this);
    get_outputlist()->delete_module_outputs(this);
#endif
}

#ifndef BARE_MODULES
void const* clockclock::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_PHASE_TRIG:
        o = &out_phase_trig;
        break;
    case outputnames::OUT_PREMOD_DEG_SIZE:
        o = &out_premod_deg_size;
        break;
    case outputnames::OUT_DEG_SIZE:
        o = &out_deg_size;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* clockclock::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_FREQ_MOD1:
        i = in_freq_mod1 = (double*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool clockclock::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_FREQ:
        set_frequency(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_FREQ_MOD1SIZE:
        set_freq_mod1size(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* clockclock::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_FREQ:
        return &hrtz_freq;
    case paramnames::PAR_FREQ_MOD1SIZE:
        return &freq_mod1size;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE clockclock::validate()
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
    return stockerrs::ERR_NO_ERROR;
}

#endif // BARE_MODULES

void clockclock::set_frequency(double f)
{
    hrtz_freq = f;
    out_premod_deg_size = freq_to_step(f, 0);
}
void clockclock::init()
{
    mod1size = freq_mod1size - 1;
}

void clockclock::run()
{
    if (freq_mod1size) {
        if (*in_freq_mod1 < 0)
            out_deg_size = out_premod_deg_size /
                           (1 + mod1size * -*in_freq_mod1);
        else
            out_deg_size = out_premod_deg_size *
                           (1 + mod1size * *in_freq_mod1);
    }
    else
        out_deg_size = out_premod_deg_size;
    degs += out_deg_size;
    if (degs >= 360) {
        degs -= 360;
        out_phase_trig = ON;
    }
    else if (out_phase_trig == ON) out_phase_trig = OFF;
}

int clockclock::clockclock_count = 0;

#ifndef BARE_MODULES
bool clockclock::done_params = false;

void clockclock::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_CLOCK, paramnames::PAR_FREQ);
    get_paramlist()->add_param(
     synthmodnames::MOD_CLOCK, paramnames::PAR_FREQ_MOD1SIZE);
    done_params = true;
}
#endif
#endif
