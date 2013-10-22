#include "../include/wave_phase.h"
#include "../include/wave_tables.h"

wave_phase::wave_phase(const char* uname) :
 synthmod::base(synthmod::WAVE_PHASE, uname, SM_HAS_OUT_OUTPUT),
 in_phase_trig(0), in_phase_step(0), in_shape_phase_step(0),
 output(0), pre_shape_output(0), play_state(OFF),
 type(wave_tables::SINE), shape_type(wave_tables::SINE),
 recycle(OFF), reset_phase(OFF),
 invert_alt(OFF), sync_shape(ON), cycles(1.0),
 phase(0), shape_phase(0), degs(0), max_degs(0), invph(1),
 table(0), shape_table(0)
{
    register_output(output::OUT_OUTPUT);
    register_output(output::OUT_PLAY_STATE);
}

void wave_phase::register_ui()
{
    register_param(param::WAVE_TYPE, wave_tables::fxstring);
    register_input(input::IN_PHASE_TRIG);
    register_input(input::IN_PHASE_STEP);
    register_param(param::INVERT_ALT);
    register_param(param::TRIG_RESET_PHASE);
    register_param(param::CYCLES);
    register_param(param::WAVE_SHAPE_TYPE, wave_tables::fxstring);
    register_input(input::IN_SHAPE_PHASE_STEP);
    register_param(param::SYNC_SHAPE);
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
        if (*in_phase_trig == ON)
        {
            play_state = ON;
            if (reset_phase == ON) {
                degs = 0;
                phase = 0;
                if (invert_alt == ON)
                    invph = -invph;
            }
            if (sync_shape == ON)
                shape_phase = 0;
        }
    }
    if (play_state == ON)
    {
        pre_shape_output =
            table[phase >> wave_tables::table_shift] * invph;
        phase += (unsigned long)
            (*in_phase_step * wave_tables::phase_step_base);
        if ((degs += *in_phase_step) > max_degs){
            degs -= max_degs;
            if (reset_phase == ON)
                phase = 0;
            if (recycle == OFF) {
                if (*in_phase_trig == OFF)
                    play_state = OFF;
            }
            if (reset_phase == OFF && invert_alt == ON)
                invph = -invph;
        }
    }
    output = pre_shape_output *
        shape_table[shape_phase >> wave_tables::table_shift];
    shape_phase += (unsigned long)
        (*in_shape_phase_step * wave_tables::phase_step_base);
}

