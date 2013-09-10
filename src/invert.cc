#ifndef INVERT_H
#include "../include/invert.h"

invert::invert(char const* uname) :
 synthmod(synthmodnames::MOD_INVERT, invert_count, uname),
 in_signal(0), out_output(0.00)
{
#ifndef BARE_MODULES
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
#endif
    invert_count++;
}

invert::~invert()
{
#ifndef BARE_MODULES
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
#endif
}

#ifndef BARE_MODULES
void const* invert::get_out(outputnames::OUT_TYPE ot)
{
    switch(ot)
    {
    case outputnames::OUT_OUTPUT:
        return &out_output;
        break;
    default:
        return 0;
    }
}

void const* invert::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal = (double*)o;
        break;
    default:
        return 0;
    }
}
#endif

int invert::invert_count = 0;
// I'm not going to be dancing here for too much longer.
#endif