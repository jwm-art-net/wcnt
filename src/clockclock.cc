#include "../include/clockclock.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

clockclock::clockclock(char const* uname) :
 synthmod(synthmodnames::CLOCK, uname, SM_DEFAULT),
 out_phase_trig(OFF),
 out_premod_phase_step(0.00), out_phase_step(0.00),
 in_freq_mod1(0), hrtz_freq(0.00), freq_mod1size(0.00), degs(360.00)
{
    // degs initialised to 360 so that it immediately triggers
    jwm.get_inputlist()->add_input(this, inputnames::IN_FREQ_MOD1);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_PHASE_TRIG);
    jwm.get_outputlist()->add_output(this,
                                    outputnames::OUT_PREMOD_PHASE_STEP);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_PHASE_STEP);
    create_params();
}

clockclock::~clockclock()
{
}

void const* clockclock::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_PHASE_TRIG:
            return &out_phase_trig;
        case outputnames::OUT_PREMOD_PHASE_STEP:
            return &out_premod_phase_step;
        case outputnames::OUT_PHASE_STEP:
            return &out_phase_step;
        default: return 0;
    }
}

void const* clockclock::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_FREQ_MOD1: return in_freq_mod1 = (double*)o;
        default: return 0;
    }
}

void const* clockclock::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_FREQ_MOD1: return in_freq_mod1;
        default: return 0;
    }
}

bool clockclock::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
    case paramnames::FREQ:
        hrtz_freq = *(double*)data;
        return true;
    case paramnames::FREQ_MOD1SIZE:
        freq_mod1size = *(double*)data;
        return true;
    default:
        return false;
    }
}

void const* clockclock::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::FREQ:          return &hrtz_freq;
        case paramnames::FREQ_MOD1SIZE: return &freq_mod1size;
        default: return 0;
    }
}

stockerrs::ERR_TYPE clockclock::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::FREQ,
                                            stockerrs::ERR_RANGE_FREQ))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::FREQ));
        invalidate();
        return stockerrs::ERR_RANGE_FREQ;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::FREQ_MOD1SIZE,
                                            stockerrs::ERR_RANGE_FMOD))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(
                                            paramnames::FREQ_MOD1SIZE));
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

bool clockclock::done_params = false;

void clockclock::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(
     synthmodnames::CLOCK, paramnames::FREQ);
    jwm.get_paramlist()->add_param(
     synthmodnames::CLOCK, paramnames::FREQ_MOD1SIZE);
    done_params = true;
}

