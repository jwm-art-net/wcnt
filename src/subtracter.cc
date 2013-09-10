#ifndef SUBTRACTER_H
#include "../include/subtracter.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"

subtracter::subtracter(char const* uname) :
 synthmod(synthmodnames::SUBTRACTER, uname),
 in_signal1(0), in_signal2(0), out_output(0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist().add_input(this, inputnames::IN_SIGNAL1);
    jwm.get_inputlist().add_input(this, inputnames::IN_SIGNAL2);
}

subtracter::~subtracter()
{
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
}

void const* subtracter::get_out(outputnames::OUT_TYPE ot) const
{
    if (ot == outputnames::OUT_OUTPUT)
        return &out_output;
    return 0;
}

void const* subtracter::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL1:     return in_signal1 = (double*)o;
        case inputnames::IN_SIGNAL2:     return in_signal2 = (double*)o;
        default: return 0;
    }
}

void const* subtracter::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL1:     return in_signal1;
        case inputnames::IN_SIGNAL2:     return in_signal2;
        default: return 0;
    }
}

#endif
