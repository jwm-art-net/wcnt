#include "../include/multiplier.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"

multiplier::multiplier(const char* uname) :
 synthmod(synthmodnames::MULTIPLIER, uname, SM_HAS_OUT_OUTPUT),
 in_signal1(0), in_signal2(0), out_output(0.00)
{
    register_input(inputnames::IN_SIGNAL1);
    register_input(inputnames::IN_SIGNAL2);
    register_output(outputnames::OUT_OUTPUT);
}

multiplier::~multiplier()
{
}

const void* multiplier::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* multiplier::set_in(inputnames::IN_TYPE it, const void* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL1:    return in_signal1 = (double*)o;
        case inputnames::IN_SIGNAL2:    return in_signal2 = (double*)o;
        default: return 0;
    }
}

const void* multiplier::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL1:    return in_signal1;
        case inputnames::IN_SIGNAL2:    return in_signal2;
        default: return 0;
    }
}

// go on... givvus a snare rush - rush - rush - rush.

