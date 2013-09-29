#include "../include/clockclock.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

clockclock::clockclock(const char* uname) :
 synthmod(module::CLOCK, uname, SM_DEFAULT),
 out_phase_trig(OFF),
 out_premod_phase_step(0.00), out_phase_step(0.00),
 in_freq_mod1(0), hrtz_freq(0.00), freq_mod1size(0.00),
 mod1size(0),degs(360.00)
{
    // degs initialised to 360 so that it immediately triggers
    register_input(input::IN_FREQ_MOD1);
    register_output(output::OUT_PHASE_TRIG);
    register_output(output::OUT_PREMOD_PHASE_STEP);
    register_output(output::OUT_PHASE_STEP);
    init_first();
}

clockclock::~clockclock()
{
}

const void* clockclock::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_PHASE_TRIG:
            return &out_phase_trig;
        case output::OUT_PREMOD_PHASE_STEP:
            return &out_premod_phase_step;
        case output::OUT_PHASE_STEP:
            return &out_phase_step;
        default: return 0;
    }
}

const void* clockclock::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_FREQ_MOD1: return in_freq_mod1 = (double*)o;
        default: return 0;
    }
}

const void* clockclock::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_FREQ_MOD1: return in_freq_mod1;
        default: return 0;
    }
}

bool clockclock::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::FREQ:
        hrtz_freq = *(double*)data;
        return true;
    case param::FREQ_MOD1SIZE:
        freq_mod1size = *(double*)data;
        return true;
    default:
        return false;
    }
}

const void* clockclock::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::FREQ:          return &hrtz_freq;
        case param::FREQ_MOD1SIZE: return &freq_mod1size;
        default: return 0;
    }
}

stockerrs::ERR_TYPE clockclock::validate()
{
    if (!jwm.get_paramlist()->validate(this, param::FREQ,
                                            stockerrs::ERR_RANGE_FREQ))
    {
        sm_err("%s", param::names::get(param::FREQ));
        invalidate();
        return stockerrs::ERR_RANGE_FREQ;
    }
    if (!jwm.get_paramlist()->validate(this, param::FREQ_MOD1SIZE,
                                            stockerrs::ERR_RANGE_FMOD))
    {
        sm_err("%s", param::names::get(
                                            param::FREQ_MOD1SIZE));
        invalidate();
        return stockerrs::ERR_RANGE_FMOD;
    }
    return stockerrs::ERR_NO_ERROR;
}

void clockclock::init()
{
    out_premod_phase_step = freq_to_step(hrtz_freq, 0);
    mod1size = freq_mod1size - 1;
}

void clockclock::run()
{
    if (freq_mod1size) {
        if (*in_freq_mod1 < 0)
            out_phase_step = out_premod_phase_step /
                           (1 + mod1size * -*in_freq_mod1);
        else
            out_phase_step = out_premod_phase_step *
                           (1 + mod1size * *in_freq_mod1);
    }
    else
        out_phase_step = out_premod_phase_step;
    degs += out_phase_step;
    if (degs >= 360) {
        degs -= 360;
        out_phase_trig = ON;
    }
    else if (out_phase_trig == ON) out_phase_trig = OFF;
}

void clockclock::init_first()
{
    if (done_first())
        return;
    register_param(param::FREQ);
    register_param(param::FREQ_MOD1SIZE);
}

