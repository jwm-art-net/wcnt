#include "../include/syncclock.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/timemap.h"

#include <math.h>

sync_clock::sync_clock(char const* uname) :
 synthmod(synthmodnames::SYNCCLOCK, uname, SM_DEFAULT),
 in_bpm(0), in_pos_stepsz(0), in_beats_per_bar(0), in_beat_value(0),
 in_phase_trig(0), in_freq_mod1(0), in_freq_mod2(0), out_phase_trig(OFF),
 out_phase_step(0), freq_mod1size(0), freq_mod2size(0), quarter_val(0),
 note_length(0), snap_to(0), premod_nlen(0), mod1size(0), mod2size(0),
 posconv(0), phaselen(0), beatlen(0),
 phasepos(0), snapto(0)
{
// degs initialised at 360 so immediately triggers if in_phase_trig is off
    jwm.get_inputlist()->add_input(this, inputnames::IN_BPM);
    jwm.get_inputlist()->add_input(this, inputnames::IN_POS_STEP_SIZE);
    jwm.get_inputlist()->add_input(this, inputnames::IN_BEATS_PER_BAR);
    jwm.get_inputlist()->add_input(this, inputnames::IN_BEAT_VALUE);
    jwm.get_inputlist()->add_input(this, inputnames::IN_PHASE_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_FREQ_MOD1);
    jwm.get_inputlist()->add_input(this, inputnames::IN_FREQ_MOD2);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_PHASE_TRIG);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_PHASE_STEP);
    create_params();
}

sync_clock::~sync_clock()
{
}

void const* sync_clock::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_PHASE_TRIG: return &out_phase_trig;
        case outputnames::OUT_PHASE_STEP: return &out_phase_step;
        default: return 0;
    }
}

void const* sync_clock::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_BPM:
            return in_bpm = (double*)o;
        case inputnames::IN_POS_STEP_SIZE:
            return in_pos_stepsz = (double*)o;
        case inputnames::IN_BEATS_PER_BAR:
            return in_beats_per_bar = (short*)o;
        case inputnames::IN_BEAT_VALUE:
            return in_beat_value = (short*)o;
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

void const* sync_clock::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_BPM:
            return in_bpm;
        case inputnames::IN_POS_STEP_SIZE:
            return in_pos_stepsz;
        case inputnames::IN_BEATS_PER_BAR:
            return in_beats_per_bar;
        case inputnames::IN_BEAT_VALUE:
            return in_beat_value;
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

bool sync_clock::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::FREQ_MOD1SIZE:
            freq_mod1size = *(double*)data;
            return true;
        case paramnames::FREQ_MOD2SIZE:
            freq_mod2size = *(double*)data;
            return true;
        case paramnames::QUARTER_VAL:
            quarter_val = *(short*)data;
            return true;
        case paramnames::NOTE_LEN:
            note_length = *(double*)data;
            return true;
        case paramnames::SNAP_TO:
            snap_to = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* sync_clock::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::FREQ_MOD1SIZE: return &freq_mod1size;
        case paramnames::FREQ_MOD2SIZE: return &freq_mod2size;
        case paramnames::QUARTER_VAL:   return &quarter_val;
        case paramnames::NOTE_LEN:      return &note_length;
        case paramnames::SNAP_TO:       return &snap_to;
        default: return 0;
    }
}

stockerrs::ERR_TYPE sync_clock::validate()
{
    if (!jwm.get_paramlist()->validate(this,
        paramnames::QUARTER_VAL, stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(
                                            paramnames::QUARTER_VAL));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_paramlist()->validate(this,
        paramnames::NOTE_LEN, stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::NOTE_LEN));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void sync_clock::init()
{
// convert note_length to quarter val as used by time map
    posconv = (double)timemap::QUARTER_VALUE / quarter_val;
    premod_nlen = note_length * posconv;
    snapto = snap_to * posconv;
    mod1size = freq_mod1size - 1;
    mod2size = freq_mod2size - 1;
}

void sync_clock::run()
{
    if (*in_phase_trig == ON) {
        phasepos = phaselen;
    }
    if (phasepos >= phaselen) {
        out_phase_trig = ON;
        phasepos -= phaselen;
        double nlen1;
        double nlen2;
        if (mod1size == 0)
            nlen1 = premod_nlen;
        else {
            if (*in_freq_mod1 < 0)
                nlen1 = premod_nlen / (1 + mod1size * -*in_freq_mod1);
            else
                nlen1 = premod_nlen * (1 + mod1size * *in_freq_mod1);
        }
        if (mod2size == 0)
            nlen2 = nlen1;
        else {
            if (*in_freq_mod2 < 0)
                nlen2 = nlen1 / (1 + mod2size * -*in_freq_mod2);
            else
                nlen2 = nlen1 * (1 + mod2size * *in_freq_mod2);
        }
        if (snapto > 0) {
            phaselen = nlen2 - fmod(nlen2, snapto);
            if (phaselen == 0)
                phaselen = snapto;
        }
        else phaselen = nlen2;
    } else if (out_phase_trig == ON) out_phase_trig = OFF;
    beatlen = (timemap::QUARTER_VALUE * (4.0 / (double)*in_beat_value));
    out_phase_step = 360.0 * (1 / ((jwm.samplerate() * (60.0 / *in_bpm)
     * (phaselen / beatlen))));
    phasepos += *in_pos_stepsz;
}

void sync_clock::create_params()
{
    if (done_params())
        return;
    modparamlist* pl = jwm.get_paramlist();
    pl->add_param(synthmodnames::SYNCCLOCK, paramnames::FREQ_MOD1SIZE);
    pl->add_param(synthmodnames::SYNCCLOCK, paramnames::FREQ_MOD2SIZE);
    pl->add_param(synthmodnames::SYNCCLOCK, paramnames::QUARTER_VAL);
    pl->add_param(synthmodnames::SYNCCLOCK, paramnames::NOTE_LEN);
    pl->add_param(synthmodnames::SYNCCLOCK, paramnames::SNAP_TO);
}

