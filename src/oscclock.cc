#include "../include/oscclock.h"
#include "../include/conversions.h"

osc_clock::osc_clock(const char* uname) :
 synthmod::base(synthmod::OSCCLOCK, uname, SM_DEFAULT),
 out_phase_trig(OFF), out_phase_step(0.00), out_premod_phase_step(0.00),
 in_freq(NULL), in_note_on_trig(NULL), in_note_slide_trig(NULL),
 in_play_state(NULL), in_freq_mod1(NULL), in_freq_mod2(NULL),
 octave_offset(0), semitones(0), freq_mod1size(1.00), freq_mod2size(1.00),
 mod1size(0.00), mod2size(0.00), degs(360.00), degsize1(0.00),
 degsize2(0.00), portamento(0.0), response_time(5.0), slide_size(0.00),
 target_phase_step(0.00),
 slidesamples(0)
{
    register_output(output::OUT_PHASE_TRIG);
    register_output(output::OUT_PREMOD_PHASE_STEP);
    register_output(output::OUT_PHASE_STEP);
}

void osc_clock::register_ui()
{
    register_input(input::IN_NOTE_ON_TRIG);
    register_input(input::IN_NOTE_SLIDE_TRIG);
    register_input(input::IN_PLAY_STATE)->set_flags(ui::UI_OPTIONAL)
                                        ->add_descr("Hint to the osc_clock to"
                                        " tell if it's driving an audible "
                                        "sound or not.");
    register_input(input::IN_FREQ);
    register_param(param::OCTAVE)->set_flags(ui::UI_OPTIONAL);
    register_param(param::TUNING_SEMITONES)->set_flags(ui::UI_OPTIONAL);
    register_input(input::IN_FREQ_MOD1) ->set_flags(ui::UI_GROUP1);
    register_param(param::FREQ_MOD1SIZE)->set_flags(ui::UI_GROUP1);
    register_input(input::IN_FREQ_MOD2) ->set_flags(ui::UI_GROUP2);
    register_param(param::FREQ_MOD2SIZE)->set_flags(ui::UI_GROUP2);
    register_param(param::PORTAMENTO)->set_flags(ui::UI_OPTIONAL);
    register_param(param::RESPONSE_TIME)->set_flags(ui::UI_OPTIONAL);
}

ui::moditem_list* osc_clock::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

osc_clock::~osc_clock()
{
}

const void* osc_clock::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_PHASE_TRIG:
            return &out_phase_trig;
        case output::OUT_PREMOD_PHASE_STEP:
            return &out_premod_phase_step;
        case output::OUT_PHASE_STEP:
            return &out_phase_step;
        default:
            return 0;
    }
}

const void* osc_clock::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_NOTE_ON_TRIG:
            return in_note_on_trig = (STATUS*)o;
        case input::IN_NOTE_SLIDE_TRIG:
            return in_note_slide_trig = (STATUS*)o;
        case input::IN_PLAY_STATE:
            return in_play_state = (STATUS*)o;
        case input::IN_FREQ:
            return in_freq = (double*)o;
        case input::IN_FREQ_MOD1:
            return in_freq_mod1 = (double*)o;
        case input::IN_FREQ_MOD2:
            return in_freq_mod2 = (double*)o;
        default:
            return 0;
    }
}

const void* osc_clock::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_NOTE_ON_TRIG:
            return in_note_on_trig;
        case input::IN_NOTE_SLIDE_TRIG:
            return in_note_slide_trig;
        case input::IN_PLAY_STATE:
            return in_play_state;
        case input::IN_FREQ:
            return in_freq;
        case input::IN_FREQ_MOD1:
            return in_freq_mod1;
        case input::IN_FREQ_MOD2:
            return in_freq_mod2;
        default:
            return 0;
    }
}

bool osc_clock::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::OCTAVE:
            octave_offset = *(wcint_t*)data;
            return true;
        case param::TUNING_SEMITONES:
            semitones = *(double*)data;
            return true;
        case param::FREQ_MOD1SIZE:
            freq_mod1size = *(double*)data;
            return true;
        case param::FREQ_MOD2SIZE:
            freq_mod2size = *(double*)data;
            return true;
        case param::PORTAMENTO:
            portamento = *(double*)data;
            return true;
        case param::RESPONSE_TIME:
            response_time = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* osc_clock::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::OCTAVE:            return &octave_offset;
        case param::TUNING_SEMITONES:  return &semitones;
        case param::FREQ_MOD1SIZE:     return &freq_mod1size;
        case param::FREQ_MOD2SIZE:     return &freq_mod2size;
        case param::PORTAMENTO:        return &portamento;
        case param::RESPONSE_TIME:     return &response_time;
        default: return 0;
    }
}

errors::TYPE osc_clock::validate()
{
    if (!validate_param(param::PORTAMENTO, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::RESPONSE_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::FREQ_MOD1SIZE, errors::RANGE_FMOD))
        return errors::RANGE_FMOD;

    if (!validate_param(param::FREQ_MOD2SIZE, errors::RANGE_FMOD))
        return errors::RANGE_FMOD;

    if (!validate_param(param::TUNING_SEMITONES, errors::RANGE_SEMI))
        return errors::RANGE_SEMI;

    if (!validate_param(param::OCTAVE, errors::RANGE_OCT))
        return errors::RANGE_OCT;

    return errors::NO_ERROR;
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
            out_premod_phase_step = freq_to_step(*in_freq, octave_offset,
                                                                semitones);
        else {
            target_phase_step = freq_to_step(*in_freq, octave_offset,
                                                                semitones);
            slidesamples = ms_to_samples(portamento);
            slide_size = (double)(target_phase_step - out_premod_phase_step)
                                                            / slidesamples;
        }
    }
    else if (*in_note_on_trig == ON) {
        if (out_premod_phase_step == 0 || response_time == 0.0
                                       || *in_play_state == OFF)
            out_premod_phase_step = freq_to_step(*in_freq, octave_offset,
                                                                semitones);
        else {
            target_phase_step = freq_to_step(*in_freq, octave_offset,
                                                                semitones);
            slidesamples = ms_to_samples(response_time);
            slide_size = (double)(target_phase_step - out_premod_phase_step)
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
    if (degs > 360.00) {
        degs -= 360.00;
        out_phase_trig = ON;
    }
    else if (out_phase_trig == ON)
        out_phase_trig = OFF;
}
