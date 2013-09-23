#include "../include/rangelimit.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

range_limit::range_limit(const char* uname) :
 synthmod(synthmodnames::RANGELIMIT, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), out_output(0), sigrangehi(0), sigrangelo(0)
{
    register_input(inputnames::IN_SIGNAL);
    register_output(outputnames::OUT_OUTPUT);
    init_first();
}

range_limit::~range_limit()
{
}

const void* range_limit::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* range_limit::set_in(inputnames::IN_TYPE it, const void* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:
            return in_signal = (double*)o;
        default:
            return 0;
    }
}

const void* range_limit::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool range_limit::set_param(paramnames::PAR_TYPE pt, const void* data)
{
    switch(pt)
    {
        case paramnames::SIG_RANGE_HI:
            sigrangehi = *(double*)data;
            return true;
        case paramnames::SIG_RANGE_LO:
            sigrangelo = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* range_limit::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::SIG_RANGE_HI: return &sigrangehi;
        case paramnames::SIG_RANGE_LO: return &sigrangelo;
        default: return 0;
    }
}

void range_limit::init()
{
    if (sigrangelo > sigrangehi) {
        double tmp = sigrangehi;
        sigrangehi = sigrangelo;
        sigrangelo = tmp;
    }
}

void range_limit::run()
{
    if ((out_output = *in_signal) < sigrangelo)
        out_output = sigrangelo;
    else if (out_output > sigrangehi)
        out_output = sigrangehi;
}

void range_limit::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::SIG_RANGE_HI);
    register_param(paramnames::SIG_RANGE_LO);
}

