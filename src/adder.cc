#include "../include/adder.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"

adder::adder(const char* uname) :
 synthmod(module::ADDER, uname, SM_HAS_OUT_OUTPUT),
 in_signal1(0), in_signal2(0), out_output(0.00)
{
    register_input(input::IN_SIGNAL1);
    register_input(input::IN_SIGNAL2);
    register_output(output::OUT_OUTPUT);
}

adder::~adder()
{
}

const void* adder::get_out(output::TYPE ot) const
{
    return (ot == output::OUT_OUTPUT) ? &out_output : 0;
}

const void* adder::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL1: return in_signal1 = (double*)o;
        case input::IN_SIGNAL2: return in_signal2 = (double*)o;
        default: return 0;
    }
}

const void* adder::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL1: return in_signal1;
        case input::IN_SIGNAL2: return in_signal2;
        default: return 0;
    }
}

