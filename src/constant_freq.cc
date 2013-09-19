#include "../include/constant_freq.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

constant_freq::constant_freq(char const* uname) :
 synthmod(synthmodnames::CONSTANT_FREQ, uname, SM_EMPTY_RUN),
 out_freq(0.0), out_phase_step(0.0)
{
    register_output(outputnames::OUT_FREQ);
    register_output(outputnames::OUT_PHASE_STEP);
    init_first();
}

constant_freq::~constant_freq()
{
}

void const* constant_freq::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot) 
    {
        case outputnames::OUT_FREQ:       return &out_freq;
        case outputnames::OUT_PHASE_STEP: return &out_phase_step;
        default: return 0;
    }
}

bool constant_freq::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt) {
        case paramnames::FREQ:
            out_freq = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* constant_freq::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::FREQ: return &out_freq;
        default: return 0;
    }
}

stockerrs::ERR_TYPE constant_freq::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::FREQ,
            stockerrs::ERR_RANGE_FREQ))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::FREQ));
        invalidate();
        return stockerrs::ERR_RANGE_FREQ;
    }
    return stockerrs::ERR_NO_ERROR;
}

void constant_freq::init()
{
    out_phase_step = freq_to_step(out_freq);
}

void constant_freq::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::FREQ);
}

