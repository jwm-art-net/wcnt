#include "../include/constant_freq.h"
#include "../include/globals.h"
#include "../include/conversions.h"

constant_freq::constant_freq(const char* uname) :
 synthmod::base(synthmod::CONSTANT_FREQ, uname, SM_EMPTY_RUN),
 out_freq(0.0), out_phase_step(0.0)
{
    register_output(output::OUT_FREQ);
    register_output(output::OUT_PHASE_STEP);
}

void constant_freq::register_ui()
{
    register_param(param::FREQ);
}

constant_freq::~constant_freq()
{
}

const void* constant_freq::get_out(output::TYPE ot) const
{
    switch(ot) 
    {
        case output::OUT_FREQ:       return &out_freq;
        case output::OUT_PHASE_STEP: return &out_phase_step;
        default: return 0;
    }
}

bool constant_freq::set_param(param::TYPE pt, const void* data)
{
    switch(pt) {
        case param::FREQ:
            out_freq = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* constant_freq::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::FREQ: return &out_freq;
        default: return 0;
    }
}

errors::TYPE constant_freq::validate()
{
    if (!validate_param(param::FREQ, errors::RANGE_FREQ))
        return errors::RANGE_FREQ;

    return errors::NO_ERROR;
}

void constant_freq::init()
{
    out_phase_step = freq_to_step(out_freq);
}


