#include "../include/impulse.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"

impulse::impulse(const char* uname) :
 synthmod(synthmodnames::IMPULSE, uname, SM_HAS_OUT_OUTPUT),
 in_trig(0), out_output(0.0)
{
    register_input(inputnames::IN_TRIG);
    register_output(outputnames::OUT_OUTPUT);
}

impulse::~impulse()
{
}

void impulse::run()
{
    out_output = (*in_trig == OFF) ? 0.0 : 1.0;
}

const void* impulse::get_out(outputnames::OUT_TYPE ot) const
{
    if (ot == outputnames::OUT_OUTPUT)
        return &out_output;
    return 0;
}

const void* impulse::set_in(inputnames::IN_TYPE it, const void* o)
{
    if (it == inputnames::IN_TRIG)
        return in_trig = (STATUS*)o;
    return 0;
}

const void* impulse::get_in(inputnames::IN_TYPE it) const
{
    if (it == inputnames::IN_TRIG)
        return in_trig;
    return 0;
}
