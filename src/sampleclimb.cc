#include "../include/sampleclimb.h"

sample_climb::sample_climb(const char* uname) :
 synthmod::base(synthmod::SAMPLECLIMB, uname, SM_HAS_OUT_OUTPUT),
 in_trig(0), in_signal(0), output(0), rate(1), target(0)
{
    register_output(output::OUT_OUTPUT);
}

void sample_climb::register_ui()
{
    register_input(input::IN_TRIG);
    register_input(input::IN_SIGNAL);
    register_param(param::RATE);
}

sample_climb::~sample_climb()
{
}

const void* sample_climb::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

const void* sample_climb::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG:   return in_trig = (STATUS*)o;
        case input::IN_SIGNAL: return in_signal= (double*)o;
        default: return 0;
    }
}

const void* sample_climb::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_TRIG:   return in_trig;
        case input::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool sample_climb::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::RATE:
            rate = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* sample_climb::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::RATE: return &rate;
        default: return 0;
    }
}

errors::TYPE sample_climb::validate()
{
    if (!validate_param(param::RATE, errors::RANGE_0_1_IN))
        return errors::RANGE_0_1_IN;

    return errors::NO_ERROR;
}

void sample_climb::run()
{
    if (*in_trig == ON)
        target = *in_signal;
    output += (target - output) * rate;
}

