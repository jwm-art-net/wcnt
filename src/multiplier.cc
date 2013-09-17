#include "../include/multiplier.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"

multiplier::multiplier(char const* uname) :
 synthmod(synthmodnames::MULTIPLIER, uname, SM_HAS_OUT_OUTPUT),
 in_signal1(0), in_signal2(0), out_output(0.00)
{
    add_output(outputnames::OUT_OUTPUT);
    add_input(inputnames::IN_SIGNAL1);
    add_input(inputnames::IN_SIGNAL2);
}

multiplier::~multiplier()
{
}

void const* multiplier::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* multiplier::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL1:    return in_signal1 = (double*)o;
        case inputnames::IN_SIGNAL2:    return in_signal2 = (double*)o;
        default: return 0;
    }
}

void const* multiplier::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL1:    return in_signal1;
        case inputnames::IN_SIGNAL2:    return in_signal2;
        default: return 0;
    }
}

// go on... givvus a snare rush - rush - rush - rush.

