#include "../include/subtracter.h"

subtracter::subtracter(const char* uname) :
 synthmod::base(synthmod::SUBTRACTER, uname, SM_HAS_OUT_OUTPUT),
 in_signal1(0), in_signal2(0), out_output(0)
{
    register_output(output::OUT_OUTPUT);
}

void subtracter::register_ui()
{
    register_input(input::IN_SIGNAL1);
    register_input(input::IN_SIGNAL2);
}

subtracter::~subtracter()
{
}

const void* subtracter::get_out(output::TYPE ot) const
{
    if (ot == output::OUT_OUTPUT)
        return &out_output;
    return 0;
}

const void* subtracter::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL1:     return in_signal1 = (double*)o;
        case input::IN_SIGNAL2:     return in_signal2 = (double*)o;
        default: return 0;
    }
}

const void* subtracter::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL1:     return in_signal1;
        case input::IN_SIGNAL2:     return in_signal2;
        default: return 0;
    }
}

