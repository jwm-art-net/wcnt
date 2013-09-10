#ifndef ADDER_H
#include "../include/adder.h"

adder::adder(char const* uname) :
 synthmod(synthmodnames::MOD_ADDER, adder_count, uname),
 in_signal(0), in_mod(0), out_output(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    get_inputlist()->add_input(this, inputnames::IN_MODIFIER);
    adder_count++;
}

adder::~adder()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* adder::get_out(outputnames::OUT_TYPE ot)
{
    if (ot == outputnames::OUT_OUTPUT)
        return &out_output;
    return 0;
}

void const* adder::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal = (double*)o;
    case inputnames::IN_MODIFIER:
        return in_mod = (double*)o;
    default:
        return 0;
    }
}

void const* adder::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal;
    case inputnames::IN_MODIFIER:
        return in_mod;
    default:
        return 0;
    }
}

void adder::run()
{
    out_output = *in_signal + *in_mod;
}

int adder::adder_count = 0;

#endif
