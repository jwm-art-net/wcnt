#ifndef SYNCCLOCK_H
#include "../include/syncclock.h"

sync_clock::sync_clock(char const* uname) :
 synthmod(synthmodnames::MOD_SYNCCLOCK, sync_clock_count, uname),
 in_bpm(0), in_pos_stepsz(0), in_beats_per_bar(0), in_beat_value(0),
 in_phase_trig(0), in_freq_mod1(0), in_freq_mod2(0), out_phase_trig(OFF),
 out_deg_size(0), freq_mod1size(0), freq_mod2size(0), quarter_val(0),
 note_length(0), snap_to(0), posconv(0), phaselen(0), beatlen(0),
 phasepos(0), snapto(0)
{
// degs initialised at 360 so immediately triggers if in_phase_trig is off
    get_inputlist()->add_input(this, inputnames::IN_BPM);
    get_inputlist()->add_input(this, inputnames::IN_POS_STEP_SIZE);
    get_inputlist()->add_input(this, inputnames::IN_BEATS_PER_BAR);
    get_inputlist()->add_input(this, inputnames::IN_BEAT_VALUE);
    get_inputlist()->add_input(this, inputnames::IN_PHASE_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_FREQ_MOD1);
    get_inputlist()->add_input(this, inputnames::IN_FREQ_MOD2);
    get_outputlist()->add_output(this, outputnames::OUT_PHASE_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_DEG_SIZE);
    sync_clock_count++;
    create_params();
}

sync_clock::~sync_clock()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* sync_clock::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot) {
    case outputnames::OUT_PHASE_TRIG:
        o = &out_phase_trig;
        break;
    case outputnames::OUT_DEG_SIZE:
        o = &out_deg_size;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* sync_clock::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it) {
    case inputnames::IN_BPM:
        i = in_bpm = (double*)o;
        break;
    case inputnames::IN_POS_STEP_SIZE:
        i = in_pos_stepsz = (double*)o;
        break;
    case inputnames::IN_BEATS_PER_BAR:
        i = in_beats_per_bar = (short*)o;
        break;
    case inputnames::IN_BEAT_VALUE:
        i = in_beat_value = (short*)o;
        break;
    case inputnames::IN_PHASE_TRIG:
        i = in_phase_trig = (STATUS*)o;
        break;
    case inputnames::IN_FREQ_MOD1:
        i = in_freq_mod1 = (double*)o;
        break;
    case inputnames::IN_FREQ_MOD2:
        i = in_freq_mod2 = (double*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool sync_clock::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt) {
    case paramnames::PAR_FREQ_MOD1SIZE:
        set_freq_mod1size(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_FREQ_MOD2SIZE:
        set_freq_mod2size(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_QUARTER_VAL:
        set_quarter_value(*(short*)data);
        retv = true;
        break;
    case paramnames::PAR_NOTE_LEN:
        set_note_length(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_SNAP_TO:
        set_snap_to(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* sync_clock::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_FREQ_MOD1SIZE:
        return &freq_mod1size;
    case paramnames::PAR_FREQ_MOD2SIZE:
        return &freq_mod2size;
    case paramnames::PAR_QUARTER_VAL:
        return &quarter_val;
    case paramnames::PAR_NOTE_LEN:
        return &note_length;
    case paramnames::PAR_SNAP_TO:
        return &snap_to;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE sync_clock::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(
        this, paramnames::PAR_QUARTER_VAL, stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_QUARTER_VAL);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(
        this, paramnames::PAR_NOTE_LEN, stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_NOTE_LEN);
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
                nlen1 = premod_nlen / (1 + mod1size * *in_freq_mod1);
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
    out_deg_size = 360.0 * (1 / ((audio_samplerate * (60.0 / *in_bpm)
     * (phaselen / beatlen))));
    phasepos += *in_pos_stepsz;
}

int sync_clock::sync_clock_count = 0;

bool sync_clock::done_params = false;

void sync_clock::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_SYNCCLOCK, paramnames::PAR_FREQ_MOD1SIZE);
    get_paramlist()->add_param(
     synthmodnames::MOD_SYNCCLOCK, paramnames::PAR_FREQ_MOD2SIZE);
    get_paramlist()->add_param(
     synthmodnames::MOD_SYNCCLOCK, paramnames::PAR_QUARTER_VAL);
    get_paramlist()->add_param(
     synthmodnames::MOD_SYNCCLOCK, paramnames::PAR_NOTE_LEN);
    get_paramlist()->add_param(
     synthmodnames::MOD_SYNCCLOCK, paramnames::PAR_SNAP_TO);
    done_params = true;
}

#endif
