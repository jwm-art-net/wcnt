#include "../include/syncclock.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/timemap.h"

#include <math.h>

sync_clock::sync_clock(const char* uname) :
 synthmod(module::SYNCCLOCK, uname, SM_DEFAULT),
 in_bpm(0), in_pos_stepsz(0), in_beats_per_bar(0), in_beat_value(0),
 in_phase_trig(0), in_freq_mod1(0), in_freq_mod2(0), out_phase_trig(OFF),
 out_phase_step(0), freq_mod1size(0), freq_mod2size(0), tpqn(0),
 note_length(0), snap_to(0), premod_nlen(0), mod1size(0), mod2size(0),
 posconv(0), phaselen(0), beatlen(0),
 phasepos(0), snapto(0)
{
// degs initialised at 360 so immediately triggers if in_phase_trig is off
    register_input(input::IN_BPM);
    register_input(input::IN_POS_STEP_SIZE);
    register_input(input::IN_BEATS_PER_BAR);
    register_input(input::IN_BEAT_VALUE);
    register_input(input::IN_PHASE_TRIG);
    register_input(input::IN_FREQ_MOD1);
    register_input(input::IN_FREQ_MOD2);
    register_output(output::OUT_PHASE_TRIG);
    register_output(output::OUT_PHASE_STEP);
    init_first();
}

sync_clock::~sync_clock()
{
}

const void* sync_clock::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_PHASE_TRIG: return &out_phase_trig;
        case output::OUT_PHASE_STEP: return &out_phase_step;
        default: return 0;
    }
}

const void* sync_clock::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_BPM:
            return in_bpm = (double*)o;
        case input::IN_POS_STEP_SIZE:
            return in_pos_stepsz = (double*)o;
        case input::IN_BEATS_PER_BAR:
            return in_beats_per_bar = (short*)o;
        case input::IN_BEAT_VALUE:
            return in_beat_value = (short*)o;
        case input::IN_PHASE_TRIG:
            return in_phase_trig = (STATUS*)o;
        case input::IN_FREQ_MOD1:
            return in_freq_mod1 = (double*)o;
        case input::IN_FREQ_MOD2:
            return in_freq_mod2 = (double*)o;
        default:
            return 0;
    }
}

const void* sync_clock::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_BPM:
            return in_bpm;
        case input::IN_POS_STEP_SIZE:
            return in_pos_stepsz;
        case input::IN_BEATS_PER_BAR:
            return in_beats_per_bar;
        case input::IN_BEAT_VALUE:
            return in_beat_value;
        case input::IN_PHASE_TRIG:
            return in_phase_trig;
        case input::IN_FREQ_MOD1:
            return in_freq_mod1;
        case input::IN_FREQ_MOD2:
            return in_freq_mod2;
        default:
            return 0;
    }
}

bool sync_clock::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::FREQ_MOD1SIZE:
            freq_mod1size = *(double*)data;
            return true;
        case param::FREQ_MOD2SIZE:
            freq_mod2size = *(double*)data;
            return true;
        case param::QUARTER_VAL:
            tpqn = *(short*)data;
            return true;
        case param::NOTE_LEN:
            note_length = *(double*)data;
            return true;
        case param::SNAP_TO:
            snap_to = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* sync_clock::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::FREQ_MOD1SIZE: return &freq_mod1size;
        case param::FREQ_MOD2SIZE: return &freq_mod2size;
        case param::QUARTER_VAL:   return &tpqn;
        case param::NOTE_LEN:      return &note_length;
        case param::SNAP_TO:       return &snap_to;
        default: return 0;
    }
}

stockerrs::ERR_TYPE sync_clock::validate()
{
    if (!jwm.get_paramlist()->validate(this,
        param::QUARTER_VAL, stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", param::names::get(param::QUARTER_VAL));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_paramlist()->validate(this,
        param::NOTE_LEN, stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", param::names::get(param::NOTE_LEN));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void sync_clock::init()
{
// convert note_length to quarter val as used by time map
    posconv = (double)timemap::TPQN / tpqn;
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
    beatlen = (timemap::TPQN * (4.0 / (double)*in_beat_value));
    out_phase_step = 360.0 * (1 / ((jwm.samplerate() * (60.0 / *in_bpm)
     * (phaselen / beatlen))));
    phasepos += *in_pos_stepsz;
}

void sync_clock::init_first()
{
    if (done_first())
        return;
    register_param(param::FREQ_MOD1SIZE);
    register_param(param::FREQ_MOD2SIZE);
    register_param(param::QUARTER_VAL);
    register_param(param::NOTE_LEN);
    register_param(param::SNAP_TO);
}

