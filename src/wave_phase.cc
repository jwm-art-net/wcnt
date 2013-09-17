#include "../include/wave_phase.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/wave_tables.h"
#include "../include/fxsparamlist.h"

wave_phase::wave_phase(char const* uname) :
 synthmod(synthmodnames::WAVE_PHASE, uname, SM_HAS_OUT_OUTPUT),
 in_phase_trig(0), in_phase_step(0), in_shape_phase_step(0),
 output(0), pre_shape_output(0), play_state(OFF),
 type(wave_tables::SINE), shape_type(wave_tables::SINE),
 recycle(OFF), reset_phase(OFF),
 invert_alt(OFF), sync_shape(ON), cycles(1.0),
 phase(0), shape_phase(0), degs(0), max_degs(0), invph(1),
 table(0), shape_table(0)
{
    add_output(outputnames::OUT_OUTPUT);
    add_output(outputnames::OUT_PLAY_STATE);
    add_input(inputnames::IN_PHASE_TRIG);
    add_input(inputnames::IN_PHASE_STEP);
    add_input(inputnames::IN_SHAPE_PHASE_STEP);
    create_params();
}

wave_phase::~wave_phase()
{
}

void const* wave_phase::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT:       return &output;
        case outputnames::OUT_PRE_SHAPE_OUTPUT:
            return &pre_shape_output;
        case outputnames::OUT_PLAY_STATE:   return &play_state;
        default: return 0;
    }
}

void const* wave_phase::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_PHASE_TRIG:
            return in_phase_trig = (STATUS*)o;
        case inputnames::IN_PHASE_STEP:
            return in_phase_step = (double*)o;
        case inputnames::IN_SHAPE_PHASE_STEP:
            return in_shape_phase_step = (double*)o;
        default: return 0;
    }
}

void const* wave_phase::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_PHASE_TRIG:       return in_phase_trig;
        case inputnames::IN_PHASE_STEP:       return in_phase_step;
        case inputnames::IN_SHAPE_PHASE_STEP: return in_shape_phase_step;
        default: return 0;
    }
}

bool wave_phase::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::WAVE_TYPE:
            type = (wave_tables::WT)(*(int*)data);
            return true;
        case paramnames::WAVE_SHAPE_TYPE:
            shape_type = (wave_tables::WT)(*(int*)data);
            return true;
        case paramnames::RECYCLE_MODE:
            recycle = *(STATUS*)data;
            return true;
        case paramnames::TRIG_RESET_PHASE:
            reset_phase = *(STATUS*)data;
            return true;
        case paramnames::CYCLES:
            cycles = *(double*)data;
            return true;
        case paramnames::SYNC_SHAPE:
            sync_shape = *(STATUS*)data;
            return true;
        case paramnames::INVERT_ALT:
            invert_alt = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

void const* wave_phase::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::WAVE_TYPE:         return &type;
        case paramnames::WAVE_SHAPE_TYPE:   return &shape_type;
        case paramnames::RECYCLE_MODE:      return &recycle;
        case paramnames::TRIG_RESET_PHASE:  return &reset_phase;
        case paramnames::CYCLES:            return &cycles;
        case paramnames::SYNC_SHAPE:        return &sync_shape;
        case paramnames::INVERT_ALT:        return &invert_alt;
        default: return 0;
    }
}

void wave_phase::init()
{
    table = jwm.get_wave_tables()->get_table(type);
    shape_table = jwm.get_wave_tables()->get_table(shape_type);
    max_degs = 360 * cycles;
    if (invert_alt == ON)
        invph = -1;
}

stockerrs::ERR_TYPE wave_phase::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::CYCLES,
            stockerrs::ERR_NEG_ZERO))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::CYCLES));
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    return stockerrs::ERR_NO_ERROR;
}
#include<iostream>
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

void wave_phase::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::WAVE_TYPE, jwm.get_wave_tables()->fxstring);
    relate_param(paramnames::WAVE_SHAPE_TYPE,
                                        jwm.get_wave_tables()->fxstring);
    relate_param(paramnames::TRIG_RESET_PHASE);
    relate_param(paramnames::RECYCLE_MODE);
    relate_param(paramnames::SYNC_SHAPE);
    relate_param(paramnames::INVERT_ALT);
    relate_param(paramnames::CYCLES);
}

