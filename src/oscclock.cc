#include "../include/oscclock.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

osc_clock::osc_clock(char const* uname) :
 synthmod(synthmodnames::OSCCLOCK, uname, SM_DEFAULT),
 out_phase_trig(OFF), out_phase_step(0.00), out_premod_phase_step(0.00),
 in_freq(NULL), in_note_on_trig(NULL), in_note_slide_trig(NULL),
 in_play_state(NULL), in_freq_mod1(NULL), in_freq_mod2(NULL),
 octave_offset(0), semitones(0), freq_mod1size(0.00), freq_mod2size(0.00),
 mod1size(0.00), mod2size(0.00), degs(360.00), degsize1(0.00),
 degsize2(0.00), portamento(0.0), response_time(0.0), slide_size(0.00),
 target_phase_step(0.00),
 slidesamples(0)
{
// degs initialised with 360 so that it 
// immediately triggers if in_phase_trig is off
    register_output(outputnames::OUT_PHASE_TRIG);
    register_output(outputnames::OUT_PREMOD_PHASE_STEP);
    register_output(outputnames::OUT_PHASE_STEP);
    register_input(inputnames::IN_NOTE_ON_TRIG);
    register_input(inputnames::IN_NOTE_SLIDE_TRIG);
    register_input(inputnames::IN_PLAY_STATE);
    register_input(inputnames::IN_FREQ);
    register_input(inputnames::IN_FREQ_MOD1);
    register_input(inputnames::IN_FREQ_MOD2);
    init_first();
}

osc_clock::~osc_clock()
{
}

void const* osc_clock::get_out(outputnames::OUT_TYPE ot) const
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

void const* osc_clock::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_NOTE_ON_TRIG:
            return in_note_on_trig = (STATUS*)o;
        case inputnames::IN_NOTE_SLIDE_TRIG:
            return in_note_slide_trig = (STATUS*)o;
        case inputnames::IN_PLAY_STATE:
            return in_play_state = (STATUS*)o;
        case inputnames::IN_FREQ:
            return in_freq = (double*)o;
        case inputnames::IN_FREQ_MOD1:
            return in_freq_mod1 = (double*)o;
        case inputnames::IN_FREQ_MOD2:
            return in_freq_mod2 = (double*)o;
        default:
            return 0;
    }
}

void const* osc_clock::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_NOTE_ON_TRIG:
            return in_note_on_trig;
        case inputnames::IN_NOTE_SLIDE_TRIG:
            return in_note_slide_trig;
        case inputnames::IN_PLAY_STATE:
            return in_play_state;
        case inputnames::IN_FREQ:
            return in_freq;
        case inputnames::IN_FREQ_MOD1:
            return in_freq_mod1;
        case inputnames::IN_FREQ_MOD2:
            return in_freq_mod2;
        default:
            return 0;
    }
}

bool osc_clock::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::OCTAVE:
            octave_offset = *(short*)data;
            return true;
        case paramnames::TUNING_SEMITONES:
            semitones = *(double*)data;
            return true;
        case paramnames::FREQ_MOD1SIZE:
            freq_mod1size = *(double*)data;
            return true;
        case paramnames::FREQ_MOD2SIZE:
            freq_mod2size = *(double*)data;
            return true;
        case paramnames::PORTAMENTO:
            portamento = *(double*)data;
            return true;
        case paramnames::RESPONSE_TIME:
            response_time = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* osc_clock::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::OCTAVE:            return &octave_offset;
        case paramnames::TUNING_SEMITONES:  return &semitones;
        case paramnames::FREQ_MOD1SIZE:     return &freq_mod1size;
        case paramnames::FREQ_MOD2SIZE:     return &freq_mod2size;
        case paramnames::PORTAMENTO:        return &portamento;
        case paramnames::RESPONSE_TIME:     return &response_time;
        default: return 0;
    }
}

stockerrs::ERR_TYPE osc_clock::validate()
{
    modparamlist* pl = jwm.get_paramlist();
    if (!pl->validate(this, paramnames::PORTAMENTO,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::PORTAMENTO);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::RESPONSE_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg
         = jwm.get_paramnames()->get_name(paramnames::RESPONSE_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::FREQ_MOD1SIZE,
            stockerrs::ERR_RANGE_FMOD))
    {
        *err_msg
         = jwm.get_paramnames()->get_name(paramnames::FREQ_MOD1SIZE);
        invalidate();
        return stockerrs::ERR_RANGE_FMOD;
    }
    if (!pl->validate(this, paramnames::FREQ_MOD2SIZE,
            stockerrs::ERR_RANGE_FMOD))
    {
        *err_msg
         = jwm.get_paramnames()->get_name(paramnames::FREQ_MOD2SIZE);
        invalidate();
        return stockerrs::ERR_RANGE_FMOD;
    }
    if (!pl->validate(this, paramnames::TUNING_SEMITONES,
            stockerrs::ERR_RANGE_SEMI))
    {
        *err_msg
         = jwm.get_paramnames()->get_name(paramnames::TUNING_SEMITONES);
        invalidate();
        return stockerrs::ERR_RANGE_SEMI;
    }
    if (!pl->validate(this, paramnames::OCTAVE,
            stockerrs::ERR_RANGE_OCT))
    {
        *err_msg
         = jwm.get_paramnames()->get_name(paramnames::OCTAVE);
        invalidate();
        return stockerrs::ERR_RANGE_OCT;
    }
    return stockerrs::ERR_NO_ERROR;
}

void osc_clock::init()
{
    mod1size = freq_mod1size - 1;
    mod2size = freq_mod2size - 1;
}

void osc_clock::run()
{
    if (*in_note_slide_trig == ON)
    {
        if (portamento == 0)
            out_premod_phase_step 
             = freq_to_step(*in_freq, octave_offset, semitones);
        else {
            target_phase_step
             = freq_to_step(*in_freq, octave_offset, semitones);
            slidesamples = ms_to_samples(portamento);
            slide_size = (double)
                (target_phase_step - out_premod_phase_step)
                    / slidesamples;
        }
    }
    else if (*in_note_on_trig == ON) {
        if (out_premod_phase_step == 0 || response_time == 0.0
            || *in_play_state == OFF)
            out_premod_phase_step 
             = freq_to_step(*in_freq, octave_offset, semitones);
        else {
            target_phase_step
             =  freq_to_step(*in_freq, octave_offset, semitones);
            slidesamples = ms_to_samples(response_time);
            slide_size = (double)
                (target_phase_step - out_premod_phase_step)
                    / slidesamples;
        }
    }
    if (slidesamples > 0)
    {
        out_premod_phase_step += slide_size;
        slidesamples--;
    }
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
    if (degs > 360.00)
    {
        degs -= 360.00;
        out_phase_trig = ON;
    }
    else if (out_phase_trig == ON)
        out_phase_trig = OFF;
}

void osc_clock::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::OCTAVE);
    register_param(paramnames::TUNING_SEMITONES);
    register_param(paramnames::PORTAMENTO);
    register_param(paramnames::RESPONSE_TIME);
    register_param(paramnames::FREQ_MOD1SIZE);
    register_param(paramnames::FREQ_MOD2SIZE);
}

