#ifndef SAMPLEHOLD_H
#include "../include/samplehold.h"

sample_hold::sample_hold(char const* uname) :
 synthmod(synthmodnames::MOD_SAMPLEHOLD, sample_hold_count, uname),
 in_trig(0), in_signal(0), output(0.00), decay_time(0.00), 
 decay_samps(0), ds(0), decay_size(0.00)
{
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_inputlist()->add_input(this, inputnames::IN_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    sample_hold_count++;
    create_params();
}

sample_hold::~sample_hold()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* sample_hold::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_OUTPUT:
        o = &output;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* sample_hold::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_TRIG:
        return in_trig = (STATUS*)o;
    case inputnames::IN_SIGNAL:
        return in_signal= (double*)o;
    default:
        return 0;
    }
}

void const* sample_hold::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_TRIG:
        return in_trig;
    case inputnames::IN_SIGNAL:
        return in_signal;
    default:
        return 0;
    }
}

bool sample_hold::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_DECAY_TIME:
        set_decay_time(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* sample_hold::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_DECAY_TIME:
        return &decay_time;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE sample_hold::validate()
{
    if (!get_paramlist()->validate(this, paramnames::PAR_DECAY_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_DECAY_TIME);
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

int sample_hold::sample_hold_count = 0;

bool sample_hold::done_params = false;

void sample_hold::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLEHOLD, paramnames::PAR_DECAY_TIME);
    done_params = true;
}

#endif
