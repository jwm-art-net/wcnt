#ifndef SAMPLECLIMB_H
#include "../include/sampleclimb.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

sample_climb::sample_climb(char const* uname) :
 synthmod(synthmodnames::SAMPLECLIMB, uname, SM_HAS_OUT_OUTPUT),
 in_trig(0), in_signal(0), output(0), rate(1), target(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    create_params();
}

sample_climb::~sample_climb()
{
}

void const* sample_climb::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

void const* sample_climb::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG:   return in_trig = (STATUS*)o;
        case inputnames::IN_SIGNAL: return in_signal= (double*)o;
        default: return 0;
    }
}

void const* sample_climb::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG:   return in_trig;
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool sample_climb::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::RATE:
            rate = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* sample_climb::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::RATE: return &rate;
        default: return 0;
    }
}

stockerrs::ERR_TYPE sample_climb::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::RATE,
            stockerrs::ERR_RANGE_0_1_IN))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::RATE);
        invalidate();
        return stockerrs::ERR_RANGE_0_1_IN;
    }
    return stockerrs::ERR_NO_ERROR;
}

void sample_climb::run()
{
    if (*in_trig == ON)
        target = *in_signal;
    output += (target - output) * rate;
}

bool sample_climb::done_params = false;

void sample_climb::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::SAMPLECLIMB, paramnames::RATE);
    done_params = true;
}

#endif
