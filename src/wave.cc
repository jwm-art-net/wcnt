#include "../include/wave.h"
#include "../include/dtr.h"
#include "../include/wave_tables.h"

wave::wave(const char* uname) :
 synthmod::base(synthmod::WAVE, uname, SM_HAS_OUT_OUTPUT),
 output(0.00), in_phase_step(NULL), type(wave_tables::ONE), phase(0),
 table(0)
{
    register_output(output::OUT_OUTPUT);
}

void wave::register_ui()
{
    register_param(param::WAVE_TYPE, wave_tables::fxstring);
    register_input(input::IN_PHASE_STEP);
}

wave::~wave()
{
}

const void* wave::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

const void* wave::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_PHASE_STEP: return in_phase_step = (double*)o;
        default: return 0;
    }
}

const void* wave::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_PHASE_STEP: return in_phase_step;
        default: return 0;
    }
}

bool wave::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::WAVE_TYPE:
            type = (wave_tables::WT)(*(int*)data);
            return true;
        default:
            return false;
    }
}

const void* wave::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::WAVE_TYPE: return &type;
        default: return 0;
    }
}

void wave::init()
{
    if (!(table = wave_tables::get_table(type))) {
        invalidate();
        return;
    }
}

void wave::run()
{
    output = table[phase >> wave_tables::table_shift];
    phase += (unsigned long)
        (*in_phase_step * wave_tables::phase_step_base);
}
