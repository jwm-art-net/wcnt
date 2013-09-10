#ifndef MULTIPLIER_H
#include "../include/multiplier.h"

multiplier::multiplier(char const* uname) :
 synthmod(synthmodnames::MOD_MULTIPLIER, multiplier_count, uname),
 in_signal(0), in_mod(0), out_output(0.00)
{
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    get_inputlist()->add_input(this, inputnames::IN_MODIFIER);
    multiplier_count++;
    // no params, no bother
}

multiplier::~multiplier()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* multiplier::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_OUTPUT:
        o = &out_output;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* multiplier::set_in(inputnames::IN_TYPE it, void const* o)
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

void const* multiplier::get_in(inputnames::IN_TYPE it)
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

int multiplier::multiplier_count = 0;
// go on, give us a snare rush rush rush rush.
#endif
