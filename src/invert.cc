#include "../include/invert.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"

invert::invert(char const* uname) :
 synthmod(synthmodnames::INVERT, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), out_output(0.00)
{
    add_output(outputnames::OUT_OUTPUT);
    add_input(inputnames::IN_SIGNAL);
}

invert::~invert()
{
}

void const* invert::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* invert::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

void const* invert::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

// I'm not going to be dancing here for too much longer.
