#ifndef COSINEWAVE_H
#include "../include/cosinewave.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/dtr.h"

#include <math.h>

cosine_wave::cosine_wave(char const* uname) :
 synthmod(synthmodnames::COSINEWAVE, uname),
 output(0.00), play_state(OFF), in_phase_trig(NULL), in_phase_step(NULL),
 degs(0.00), recycle(OFF), reset_phase(OFF), cycles(0), maxdegs(0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_outputlist().add_output(this, outputnames::OUT_PLAY_STATE);
    jwm.get_inputlist().add_input(this, inputnames::IN_PHASE_TRIG);
    jwm.get_inputlist().add_input(this, inputnames::IN_PHASE_STEP);
    create_params();
}
cosine_wave::~cosine_wave()
{
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
}

void const* cosine_wave::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT:       return &output;
        case outputnames::OUT_PLAY_STATE:   return &play_state;
        default: return 0;
    }
}

void const* cosine_wave::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_PHASE_TRIG: return in_phase_trig = (STATUS*)o;
        case inputnames::IN_PHASE_STEP: return in_phase_step = (double*)o;
        default: return 0;
    }
}

void const* cosine_wave::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_PHASE_TRIG: return in_phase_trig;
        case inputnames::IN_PHASE_STEP: return in_phase_step;
        default: return 0;
    }
}

bool cosine_wave::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::RECYCLE_MODE:
            recycle = *(STATUS*)data;
            return true;
        case paramnames::TRIG_RESET_PHASE:
            reset_phase = *(STATUS*)data;
            return true;
        case paramnames::CYCLES:
            cycles = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* cosine_wave::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::RECYCLE_MODE:      return &recycle;
        case paramnames::TRIG_RESET_PHASE:  return &reset_phase;
        case paramnames::CYCLES:            return &cycles;
        default: return 0;
    }
}

stockerrs::ERR_TYPE cosine_wave::validate()
{
    if (!jwm.get_paramlist().validate(this, paramnames::CYCLES,
            stockerrs::ERR_NEG_ZERO))
    {
        *err_msg = jwm.get_paramnames().get_name(paramnames::CYCLES);
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    return stockerrs::ERR_NO_ERROR;
}

void cosine_wave::init()
{
    maxdegs = cycles * 360.00;
}

void cosine_wave::run()
{
    if (*in_phase_trig == ON) {
        play_state = ON;
        if (reset_phase == ON) degs = 0.00;
    }
    if (play_state == ON)
        output = cos(degs * DTR);
    degs += *in_phase_step;
    if (degs > maxdegs) {
        degs -= maxdegs;
        if (play_state == ON && recycle == OFF) {
            if (*in_phase_trig == ON) {
                if (reset_phase == ON)
                    degs = 0.00;
            }
            else
                play_state = OFF;
        }
    }
}

bool cosine_wave::done_params = false;

void cosine_wave::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist().add_param(
     synthmodnames::COSINEWAVE, paramnames::RECYCLE_MODE);
    jwm.get_paramlist().add_param(
     synthmodnames::COSINEWAVE, paramnames::TRIG_RESET_PHASE);
    jwm.get_paramlist().add_param(
     synthmodnames::COSINEWAVE, paramnames::CYCLES);
    done_params = true;
}

#endif
