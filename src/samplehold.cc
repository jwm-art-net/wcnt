#include "../include/samplehold.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

sample_hold::sample_hold(char const* uname) :
 synthmod(synthmodnames::SAMPLEHOLD, uname, SM_HAS_OUT_OUTPUT),
 in_trig(0), in_signal(0), output(0.00), decay_time(0.00), 
 decay_samps(0), ds(0), decay_size(0.00)
{
    add_output(outputnames::OUT_OUTPUT);
    add_input(inputnames::IN_TRIG);
    add_input(inputnames::IN_SIGNAL);
    create_params();
}

sample_hold::~sample_hold()
{
}

void const* sample_hold::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

void const* sample_hold::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG:   return in_trig = (STATUS*)o;
        case inputnames::IN_SIGNAL: return in_signal= (double*)o;
        default: return 0;
    }
}

void const* sample_hold::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG:   return in_trig;
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool sample_hold::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::DECAY_TIME:
            decay_time = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* sample_hold::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::DECAY_TIME: return &decay_time;
        default: return 0;
    }
}

stockerrs::ERR_TYPE sample_hold::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::DECAY_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(
                                            paramnames::DECAY_TIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void sample_hold::init()
{
    decay_samps = ms_to_samples(decay_time);
    ds = 0;
}

void sample_hold::run()
{
    if (*in_trig == ON)
    {
        output = *in_signal;
        if (decay_time > 0) {
            decay_size = output / decay_samps;
            ds = decay_samps;
        }
    }
    else if (ds > 0)
    {
        output -= decay_size;
        ds--;
        if (ds == 0)
            output = 0;
    }
}

void sample_hold::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::DECAY_TIME);
}

