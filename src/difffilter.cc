#include "../include/difffilter.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

diff_filter::diff_filter(char const* uname) :
 synthmod(synthmodnames::DIFFFILTER, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), out_output(0.0), oldinsig(0.0)
{
    register_input(inputnames::IN_SIGNAL);
    register_output(outputnames::OUT_OUTPUT);
}

diff_filter::~diff_filter()
{
}

void diff_filter::run()
{
    double insig = *in_signal;
    out_output = insig - oldinsig;
    oldinsig = insig;
}

void const* diff_filter::get_out(outputnames::OUT_TYPE ot) const
{
    if (ot == outputnames::OUT_OUTPUT)
        return &out_output;
    return 0;
}

void const* diff_filter::set_in(inputnames::IN_TYPE it, void const* o)
{
    if (it == inputnames::IN_SIGNAL)
        return in_signal = (double*)o;
    return 0;
}

void const* diff_filter::get_in(inputnames::IN_TYPE it) const
{
    if (it == inputnames::IN_SIGNAL)
        return in_signal;
    return 0;
}

