#include "../include/lfoclock.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

lfo_clock::lfo_clock(char const* uname) :
 synthmod(synthmodnames::LFOCLOCK, uname, SM_HAS_OUT_OUTPUT),
 out_phase_trig(OFF), out_phase_step(0.00), out_premod_phase_step(0.00),
 note_length_freq(0), hrtz_freq(0.00), in_phase_trig(NULL), 
 in_freq_mod1(NULL), in_freq_mod2(NULL), freq_mod1size(0.00),
 freq_mod2size(0.00), mod1size(0), mod2size(0), degs(360.00),
 degsize1(0.00), degsize2(0.00)
{
// degs initialised at 360 so immediately triggers if in_phase_trig is off
    register_input(inputnames::IN_PHASE_TRIG);
    register_input(inputnames::IN_FREQ_MOD1);
    register_input(inputnames::IN_FREQ_MOD2);
    register_output(outputnames::OUT_PHASE_TRIG);
    register_output(outputnames::OUT_PREMOD_PHASE_STEP);
    register_output(outputnames::OUT_PHASE_STEP);
    init_first();
}

lfo_clock::~lfo_clock()
{
}

void const* lfo_clock::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_PHASE_TRIG:
            return &out_phase_trig;
        case outputnames::OUT_PREMOD_PHASE_STEP:
            return &out_premod_phase_step;
        case outputnames::OUT_PHASE_STEP:
            return &out_phase_step;
        default:
            return 0;
    }
}

void const* lfo_clock::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
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

void const* lfo_clock::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_PHASE_TRIG: return in_phase_trig;
        case inputnames::IN_FREQ_MOD1:  return in_freq_mod1;
        case inputnames::IN_FREQ_MOD2:  return in_freq_mod2;
        default: return 0;
    }
}

bool lfo_clock::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt) {
    case paramnames::FREQ_MOD1SIZE:
        freq_mod1size = *(double*)data;
        return true;
    case paramnames::FREQ_MOD2SIZE:
        freq_mod2size = *(double*)data;
        return true;
    case paramnames::FREQ:
        hrtz_freq = *(double*)data;
        return true;
    default:
        return false;
    }
}

void const* lfo_clock::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::FREQ_MOD1SIZE: return &freq_mod1size;
        case paramnames::FREQ_MOD2SIZE: return &freq_mod2size;
        case paramnames::FREQ:          return &hrtz_freq;
        default: return 0;
    }
}

stockerrs::ERR_TYPE lfo_clock::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::FREQ,
            stockerrs::ERR_RANGE_FREQ))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::FREQ);
        invalidate();
        return stockerrs::ERR_RANGE_FREQ;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::FREQ_MOD1SIZE,
            stockerrs::ERR_RANGE_FMOD))
    {
        *err_msg
         = jwm.get_paramnames()->get_name(paramnames::FREQ_MOD1SIZE);
        invalidate();
        return stockerrs::ERR_RANGE_FMOD;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::FREQ_MOD2SIZE,
            stockerrs::ERR_RANGE_FMOD))
    {
        *err_msg
         = jwm.get_paramnames()->get_name(paramnames::FREQ_MOD2SIZE);
        invalidate();
        return stockerrs::ERR_RANGE_FMOD;
    }
    return stockerrs::ERR_NO_ERROR;
}

void lfo_clock::init()
{
    out_premod_phase_step = freq_to_step(hrtz_freq, 0);
    mod1size = freq_mod1size - 1;
    mod2size = freq_mod2size - 1;
}

void lfo_clock::run()
{
    if (mod1size == 0)
        degsize1 = out_premod_phase_step;
    else {
        if (*in_freq_mod1 < 0)
            degsize1 = out_premod_phase_step
             / (1 + mod1size * -*in_freq_mod1);
        else
            degsize1 = out_premod_phase_step
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
    out_phase_step = degsize2;
    degs += out_phase_step;
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

void lfo_clock::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::FREQ);
    register_param(paramnames::FREQ_MOD1SIZE);
    register_param(paramnames::FREQ_MOD2SIZE);
}

