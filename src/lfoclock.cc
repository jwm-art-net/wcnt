#include "../include/lfoclock.h"
#include "../include/conversions.h"

lfo_clock::lfo_clock(const char* uname) :
 synthmod::base(synthmod::LFOCLOCK, uname, SM_HAS_OUT_OUTPUT),
 out_phase_trig(OFF), out_phase_step(0.00), out_premod_phase_step(0.00),
 note_length_freq(0), hrtz_freq(0.00), in_phase_trig(NULL),
 in_freq_mod1(NULL), in_freq_mod2(NULL), freq_mod1size(1.00),
 freq_mod2size(1.00), mod1size(0), mod2size(0), degs(360.00),
 degsize1(0.00), degsize2(0.00)
{
    register_output(output::OUT_PHASE_TRIG);
    register_output(output::OUT_PREMOD_PHASE_STEP);
    register_output(output::OUT_PHASE_STEP);
}

void lfo_clock::register_ui()
{
    register_param(param::FREQ);
    register_input(input::IN_PHASE_TRIG);
    register_input(input::IN_FREQ_MOD1) ->set_flags(ui::UI_OPTIONAL |
                                                    ui::UI_SET1);
    register_param(param::FREQ_MOD1SIZE)->set_flags(ui::UI_SET1);

    register_input(input::IN_FREQ_MOD2) ->set_flags(ui::UI_OPTIONAL |
                                                    ui::UI_SET2);
    register_param(param::FREQ_MOD2SIZE)->set_flags(ui::UI_SET2);
}

ui::moditem_list* lfo_clock::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

lfo_clock::~lfo_clock()
{
}

const void* lfo_clock::get_out(output::TYPE ot) const
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

const void* lfo_clock::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
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

const void* lfo_clock::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_PHASE_TRIG: return in_phase_trig;
        case input::IN_FREQ_MOD1:  return in_freq_mod1;
        case input::IN_FREQ_MOD2:  return in_freq_mod2;
        default: return 0;
    }
}

bool lfo_clock::set_param(param::TYPE pt, const void* data)
{
    switch(pt) {
    case param::FREQ_MOD1SIZE:
        freq_mod1size = *(double*)data;
        return true;
    case param::FREQ_MOD2SIZE:
        freq_mod2size = *(double*)data;
        return true;
    case param::FREQ:
        hrtz_freq = *(double*)data;
        return true;
    default:
        return false;
    }
}

const void* lfo_clock::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::FREQ_MOD1SIZE: return &freq_mod1size;
        case param::FREQ_MOD2SIZE: return &freq_mod2size;
        case param::FREQ:          return &hrtz_freq;
        default: return 0;
    }
}

errors::TYPE lfo_clock::validate()
{
    if (!validate_param(param::FREQ, errors::RANGE_FREQ))
        return errors::RANGE_FREQ;

    if (!validate_param(param::FREQ_MOD1SIZE, errors::RANGE_FMOD))
        return errors::RANGE_FMOD;

    if (!validate_param(param::FREQ_MOD2SIZE, errors::RANGE_FMOD))
        return errors::RANGE_FMOD;

    return errors::NO_ERROR;
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

