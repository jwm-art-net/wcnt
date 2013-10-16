#include "../include/samplehold.h"
#include "../include/conversions.h"

sample_hold::sample_hold(const char* uname) :
 synthmod::base(synthmod::SAMPLEHOLD, uname, SM_HAS_OUT_OUTPUT),
 in_trig(0), in_signal(0), output(0.00), decay_time(0.00), 
 decay_samps(0), ds(0), decay_size(0.00)
{
    register_output(output::OUT_OUTPUT);
}

void sample_hold::register_ui()
{
    register_input(input::IN_TRIG);
    register_input(input::IN_SIGNAL);
    register_param(param::DECAY_TIME);
}

sample_hold::~sample_hold()
{
}

const void* sample_hold::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

const void* sample_hold::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG:   return in_trig = (STATUS*)o;
        case input::IN_SIGNAL: return in_signal= (double*)o;
        default: return 0;
    }
}

const void* sample_hold::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_TRIG:   return in_trig;
        case input::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool sample_hold::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::DECAY_TIME:
            decay_time = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* sample_hold::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::DECAY_TIME: return &decay_time;
        default: return 0;
    }
}

errors::TYPE sample_hold::validate()
{
    if (!validate_param(param::DECAY_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    return errors::NO_ERROR;
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


