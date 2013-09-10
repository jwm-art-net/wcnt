#ifndef WAVE_H
#include "../include/wave.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/dtr.h"
#include "../include/wave_tables.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"

wave::wave(char const* uname) :
 synthmod(synthmodnames::WAVE, uname),
 output(0.00), in_phase_step(NULL), phase(0), table(0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist().add_input(this, inputnames::IN_PHASE_STEP);
    create_params();
}
wave::~wave()
{
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
}

void const* wave::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

void const* wave::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_PHASE_STEP: return in_phase_step = (double*)o;
        default: return 0;
    }
}

void const* wave::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_PHASE_STEP: return in_phase_step;
        default: return 0;
    }
}

bool wave::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::WAVE_TYPE:
            type = (wave_tables::WT)(*(int*)data);
            return true;
        default:
            return false;
    }
}

void const* wave::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::WAVE_TYPE: return &type;
        default: return 0;
    }
}

void wave::init()
{
    table = jwm.get_wave_tables().get_table(type);
}

void wave::run()
{
    output = table[phase >> wave_tables::table_shift];
    phase += (unsigned long)
        (*in_phase_step * wave_tables::phase_step_base);
}

void wave::create_params()
{
    if (wave::done_params == true)
        return;
    jwm.get_paramlist().add_param(synthmodnames::WAVE,
        paramnames::WAVE_TYPE);
    jwm.get_fxsparamlist().add_param(jwm.get_wave_tables().fxstring,
                                      paramnames::WAVE_TYPE);
    wave::done_params = true;
}

bool wave::done_params = false;

#endif
