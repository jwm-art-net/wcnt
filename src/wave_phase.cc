#include "../include/wave_phase.h"
#include "../include/wave_tables.h"

wave_phase::wave_phase(const char* uname) :
 synthmod::base(synthmod::WAVE_PHASE, uname, SM_HAS_OUT_OUTPUT),
 in_phase_trig(0), in_phase_step(0), in_shape_phase_step(0),
 output(0), pre_shape_output(0), play_state(OFF),
 type(wave_tables::SINE), shape_type(wave_tables::ONE),
 recycle(OFF), reset_phase(OFF),
 invert_alt(OFF), sync_shape(ON), cycles(1.0), start_phase(0.0),
 shape_start_phase(0.0), phase_mod_size(0.0), zero_when_off(ON),
 phase(0), shape_phase(0), startphase(0), shapestartphase(0), phzamt(0),
 degs(0), max_degs(0), invph(1),
 table(0), shape_table(0)
{
    register_output(output::OUT_OUTPUT);
    register_output(output::OUT_PLAY_STATE);
}

void wave_phase::register_ui()
{
    register_param(param::WAVE_TYPE, wave_tables::fxstring);
    register_input(input::IN_PHASE_TRIG)->add_descr("the wave_phase module MUST be triggered!");
    register_input(input::IN_PHASE_STEP);
    register_param(param::TRIG_RESET_PHASE);
    register_param(param::START_PHASE)   ->set_flags(ui::UI_OPTIONAL);
    register_param(param::CYCLES)        ->set_flags(ui::UI_OPTIONAL);
    register_param(param::INVERT_ALT)    ->set_flags(ui::UI_OPTIONAL);

    register_input(input::IN_PHASE_MOD)  ->set_flags(ui::UI_GROUP1);
    register_param(param::PHASE_MOD_SIZE)->set_flags(ui::UI_GROUP1);

    register_param(param::WAVE_SHAPE_TYPE, wave_tables::fxstring)->set_flags(ui::UI_GROUP2);
    register_input(input::IN_SHAPE_PHASE_STEP)  ->set_flags(ui::UI_GROUP2);
    register_param(param::SHAPE_START_PHASE)    ->set_flags(ui::UI_GROUP2 | ui::UI_OPTIONAL);
    register_param(param::SYNC_SHAPE)           ->set_flags(ui::UI_GROUP2);

    register_param(param::ZERO_WHEN_OFF)->set_flags(ui::UI_OPTIONAL);
    register_param(param::RECYCLE_MODE);
}

ui::moditem_list* wave_phase::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

wave_phase::~wave_phase()
{
}

const void* wave_phase::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT:       return &output;
        case output::OUT_PRE_SHAPE_OUTPUT:
            return &pre_shape_output;
        case output::OUT_PLAY_STATE:   return &play_state;
        default: return 0;
    }
}

const void* wave_phase::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_PHASE_TRIG:
            return in_phase_trig = (STATUS*)o;
        case input::IN_PHASE_STEP:
            return in_phase_step = (double*)o;
        case input::IN_SHAPE_PHASE_STEP:
            return in_shape_phase_step = (double*)o;
        case input::IN_PHASE_MOD:
            return in_phase_mod = (double*)o;
        default: return 0;
    }
}

const void* wave_phase::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_PHASE_TRIG:       return in_phase_trig;
        case input::IN_PHASE_STEP:       return in_phase_step;
        case input::IN_SHAPE_PHASE_STEP: return in_shape_phase_step;
        case input::IN_PHASE_MOD:        return in_phase_mod;
        default: return 0;
    }
}

bool wave_phase::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::WAVE_TYPE:
            type = (wave_tables::WT)(*(int*)data);
            return true;
        case param::WAVE_SHAPE_TYPE:
            shape_type = (wave_tables::WT)(*(int*)data);
            return true;
        case param::RECYCLE_MODE:
            recycle = *(STATUS*)data;
            return true;
        case param::TRIG_RESET_PHASE:
            reset_phase = *(STATUS*)data;
            return true;
        case param::CYCLES:
            cycles = *(double*)data;
            return true;
        case param::SYNC_SHAPE:
            sync_shape = *(STATUS*)data;
            return true;
        case param::INVERT_ALT:
            invert_alt = *(STATUS*)data;
            return true;
        case param::START_PHASE:
            start_phase = *(double*)data;
            return true;
        case param::SHAPE_START_PHASE:
            shape_start_phase = *(double*)data;
            return true;
        case param::ZERO_WHEN_OFF:
            zero_when_off = *(STATUS*)data;
            return true;
        case param::PHASE_MOD_SIZE:
            phase_mod_size = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* wave_phase::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::WAVE_TYPE:         return &type;
        case param::WAVE_SHAPE_TYPE:   return &shape_type;
        case param::RECYCLE_MODE:      return &recycle;
        case param::TRIG_RESET_PHASE:  return &reset_phase;
        case param::CYCLES:            return &cycles;
        case param::SYNC_SHAPE:        return &sync_shape;
        case param::INVERT_ALT:        return &invert_alt;
        case param::START_PHASE:       return &start_phase;
        case param::SHAPE_START_PHASE: return &shape_start_phase;
        case param::ZERO_WHEN_OFF:     return &zero_when_off;
        case param::PHASE_MOD_SIZE:    return &phase_mod_size;
        default: return 0;
    }
}

void wave_phase::init()
{
    table = wave_tables::get_table(type);
    shape_table = wave_tables::get_table(shape_type);
    max_degs = 360 * cycles;
    if (invert_alt == ON)
        invph = -1;
    startphase = start_phase * (uint32_t)-1;
    shapestartphase = shape_start_phase * (uint32_t)-1;
    phzamt = phase_mod_size * (uint32_t)-1;
}

errors::TYPE wave_phase::validate()
{
    if (!validate_param(param::CYCLES, errors::NEG_OR_ZERO))
        return errors::NEG_OR_ZERO;

    return errors::NO_ERROR;
}

void wave_phase::run()
{
    if (play_state == OFF || reset_phase == ON) {
        if (play_state == OFF && zero_when_off == ON)
            output = 0.0;
        if (*in_phase_trig == ON)
        {
            play_state = ON;
            if (reset_phase == ON) {
                degs = 0;
                phase = startphase;
                if (invert_alt == ON)
                    invph = -invph;
            }
            if (sync_shape == ON)
                shape_phase = shapestartphase;
        }
    }
    if (play_state == ON)
    {
        uint32_t phz = (double)phase + phzamt * *in_phase_mod;
        pre_shape_output = table[phz >> wave_tables::table_shift] * invph;
        phase += (samp_t)(*in_phase_step * wave_tables::phase_step_base);
        if ((degs += *in_phase_step) > max_degs){
            degs -= max_degs;
            if (reset_phase == ON)
                phase = startphase;
            else if (invert_alt == ON)
                invph = -invph;
            if (recycle == OFF) {
                if (*in_phase_trig == OFF) {
                    play_state = OFF;
                }
            }
        }
        output = pre_shape_output *
        shape_table[shape_phase >> wave_tables::table_shift];
    }
    shape_phase += (samp_t)(*in_shape_phase_step * wave_tables::phase_step_base);
}

