#include "../include/constant_freq.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

constant_freq::constant_freq(char const* uname) :
 synthmod(synthmodnames::CONSTANT_FREQ, uname, SM_EMPTY_RUN),
 out_freq(0.0), out_phase_step(0.0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_FREQ);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_PHASE_STEP);
    create_params();
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
        *err_msg = jwm.get_paramnames()->get_name(paramnames::FREQ);
        invalidate();
        return stockerrs::ERR_RANGE_FREQ;
    }
    return stockerrs::ERR_NO_ERROR;
}

void constant_freq::init()
{
    out_phase_step = freq_to_step(out_freq);
}

bool constant_freq::done_params = false;

void constant_freq::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::CONSTANT_FREQ, paramnames::FREQ);
    done_params = true;
}

