#include "../include/subtracter.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"

subtracter::subtracter(const char* uname) :
 synthmod(synthmodnames::SUBTRACTER, uname, SM_HAS_OUT_OUTPUT),
 in_signal1(0), in_signal2(0), out_output(0)
{
    register_input(inputnames::IN_SIGNAL1);
    register_input(inputnames::IN_SIGNAL2);
    register_output(outputnames::OUT_OUTPUT);
}

subtracter::~subtracter()
{
}

const void* subtracter::get_out(outputnames::OUT_TYPE ot) const
{
    if (ot == outputnames::OUT_OUTPUT)
        return &out_output;
    return 0;
}

const void* subtracter::set_in(inputnames::IN_TYPE it, const void* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL1:     return in_signal1 = (double*)o;
        case inputnames::IN_SIGNAL2:     return in_signal2 = (double*)o;
        default: return 0;
    }
}

const void* subtracter::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL1:     return in_signal1;
        case inputnames::IN_SIGNAL2:     return in_signal2;
        default: return 0;
    }
}

