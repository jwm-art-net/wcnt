#include "../include/seqecho.h"
#include "../include/globals.h"
#include "../include/conversions.h"

seq_echo::seq_echo(const char* uname) :
 synthmod::base(synthmod::SEQ_ECHO, uname, SM_DEFAULT),
 in_note_on_trig(0), in_note_slide_trig(0), in_note_off_trig(0),
 in_freq(0), in_velocity(0), in_reset_trig(0),
 out_note_on_trig(OFF), out_note_slide_trig(OFF), out_note_off_trig(OFF),
 out_freq(0), out_velocity(0), out_index(-1),
 delay_time(0.0), count(0), send_input_out(OFF),
 on_trigs(0), slide_trigs(0), off_trigs(0), freqs(0), vels(0),
 pastmax(0), pastpos(0), last(0)
{
    register_output(output::OUT_INDEX);
    register_output(output::OUT_NOTE_ON_TRIG);
    register_output(output::OUT_NOTE_SLIDE_TRIG);
    register_output(output::OUT_NOTE_OFF_TRIG);
    register_output(output::OUT_FREQ);
    register_output(output::OUT_VELOCITY);
}

void seq_echo::register_ui()
{
    ui::moditem* i = 0;
    if ((i = register_param(param::COUNT)))
        i->add_comment("Number of echos to process.");
    register_param(param::DELAY_TIME);
    if ((i = register_input(input::IN_RESET_TRIG)))
        i->add_comment("Trigger input to kill echos.");
    register_param(param::SEND_INPUT_OUT);
    register_comment("Inputs to echo:");
    register_input(input::IN_NOTE_ON_TRIG);
    register_input(input::IN_NOTE_SLIDE_TRIG);
    register_input(input::IN_NOTE_OFF_TRIG);
    register_input(input::IN_FREQ);
    register_input(input::IN_VELOCITY);
}


seq_echo::~seq_echo()
{
    if (on_trigs)
        delete [] on_trigs;
    if (slide_trigs)
        delete [] slide_trigs;
    if (off_trigs)
        delete [] off_trigs;
    if (vels)
        delete [] vels;
    if (freqs)
        delete [] freqs;
}

void seq_echo::init()
{
    pastmax = ms_to_samples(delay_time);
    on_trigs = new wcint_t[pastmax];
    slide_trigs = new wcint_t[pastmax];
    off_trigs = new wcint_t[pastmax];
    freqs = new double[pastmax];
    vels = new double[pastmax];

    if(!on_trigs || !slide_trigs || !off_trigs || !freqs || !vels) {
        invalidate();
        return;
    }

    for (int i = 0; i < pastmax; i++) {
        on_trigs[i] = 0;
        slide_trigs[i] = 0;
        off_trigs[i] = 0;
        freqs[i] = 0.0f;
        vels[i] = 0.0f;
    }
    pastpos = pastmax - 1;
    last = count + (send_input_out == ON ? 1 : 0);
}

const void* seq_echo::get_out(output::TYPE ot) const
{
    switch(ot)
    {
    case output::OUT_INDEX:             return &out_index;
    case output::OUT_NOTE_ON_TRIG:      return &out_note_on_trig;
    case output::OUT_NOTE_SLIDE_TRIG:   return &out_note_slide_trig;
    case output::OUT_NOTE_OFF_TRIG:     return &out_note_off_trig;
    case output::OUT_FREQ:              return &out_freq;
    case output::OUT_VELOCITY:          return &out_velocity;
    default: return 0;
    }
}

const void* seq_echo::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
    case input::IN_NOTE_ON_TRIG:
        return in_note_on_trig = (const STATUS*)o;
    case input::IN_NOTE_SLIDE_TRIG:
        return in_note_slide_trig = (const STATUS*)o;
    case input::IN_NOTE_OFF_TRIG:
        return in_note_off_trig = (const STATUS*)o;
    case input::IN_FREQ:
        return in_freq = (const double*)o;
    case input::IN_VELOCITY:
        return in_velocity = (const double*)o;
    case input::IN_RESET_TRIG:
        return in_reset_trig = (const STATUS*)o;
    default:
        return 0;
    }
}

const void* seq_echo::get_in(input::TYPE it) const
{
    switch(it)
    {
    case input::IN_NOTE_ON_TRIG:    return &in_note_on_trig;
    case input::IN_NOTE_SLIDE_TRIG: return &in_note_slide_trig;
    case input::IN_NOTE_OFF_TRIG:   return &in_note_off_trig;
    case input::IN_FREQ:            return &in_freq;
    case input::IN_VELOCITY:        return &in_velocity;
    case input::IN_RESET_TRIG:      return &in_reset_trig;
    default: return 0;
    }
}

bool seq_echo::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        case param::COUNT:
            count = *(wcint_t*)data;
            return true;
        case param::SEND_INPUT_OUT:
            send_input_out = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

const void* seq_echo::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::DELAY_TIME:        return &delay_time;
        case param::COUNT:             return &count;
        case param::SEND_INPUT_OUT:    return &send_input_out;
        default: return 0;
    }
}

errors::TYPE seq_echo::validate()
{
    if (!validate_param(param::DELAY_TIME, errors::NEG_OR_ZERO))
        return errors::NEG_OR_ZERO;

    if (!validate_param(param::COUNT, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    return errors::NO_ERROR;
}

void seq_echo::run()
{
    if (*in_reset_trig == ON) {
        pastpos = 0;
        out_index = -1;
        out_note_on_trig = OFF;
        out_note_slide_trig = OFF;
        out_note_off_trig = OFF;
        out_freq = 0.0f;
        out_velocity = 0.0f;
        for (int i = 0; i < pastmax; ++i) {
            on_trigs[i] = 0;
            slide_trigs[i] = 0;
            off_trigs[i] = 0;
            freqs[i] = 0.0f;
            vels[i] = 0.0f;
        }
        return;
    }

    if (*in_note_on_trig == ON) {
        on_trigs[pastpos] = last;
        freqs[pastpos] = *in_freq;
        vels[pastpos] = *in_velocity;
        if (send_input_out == ON) {
            out_index = 0;
            out_note_on_trig = ON;
            out_freq = *in_freq;
            out_velocity = *in_velocity;
        }
    }
    else if (on_trigs[pastpos] > 0) {
        out_note_on_trig = ON;
        on_trigs[pastpos]--;
        out_freq = freqs[pastpos];
        out_velocity = (on_trigs[pastpos] / (double)count)
                                             * vels[pastpos];
        out_index = last - 1 - on_trigs[pastpos];
    }
    else if (out_note_on_trig == ON)
        out_note_on_trig = OFF;

    if (*in_note_slide_trig == ON) {
        slide_trigs[pastpos] = last;
        freqs[pastpos] = *in_freq;
        vels[pastpos] = *in_velocity;
        if (send_input_out == ON) {
            out_note_slide_trig = ON;
            out_freq = *in_freq;
            out_velocity = *in_velocity;
        }
    }
    else if (slide_trigs[pastpos] > 0) {
        out_note_slide_trig = ON;
        slide_trigs[pastpos]--;
        out_freq = freqs[pastpos];
        out_velocity = (slide_trigs[pastpos] / (double)count)
                                             * vels[pastpos];
    }
    else if (out_note_slide_trig == ON)
        out_note_slide_trig = OFF;

    if (*in_note_off_trig == ON) {
        off_trigs[pastpos] = last;
        if (send_input_out == ON)
            out_note_off_trig = ON;
    }
    else if (off_trigs[pastpos] > 0) {
        out_note_off_trig = ON;
        off_trigs[pastpos]--;
    }
    else if (out_note_off_trig == ON)
        out_note_off_trig = OFF;

    if (++pastpos == pastmax)
        pastpos = 0;
}

