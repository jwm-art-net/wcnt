#ifndef RANGELIMIT_H
#include "../include/rangelimit.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

range_limit::range_limit(char const* uname) :
 synthmod(synthmodnames::RANGELIMIT, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), out_output(0), sigrangehi(0), sigrangelo(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    create_params();
}

range_limit::~range_limit()
{
}

void const* range_limit::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* range_limit::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:
            return in_signal = (double*)o;
        default:
            return 0;
    }
}

void const* range_limit::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool range_limit::set_param(paramnames::PAR_TYPE pt, void const* data)
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

void const* range_limit::get_param(paramnames::PAR_TYPE pt) const
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

bool range_limit::done_params = false;

void range_limit::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::RANGELIMIT, paramnames::SIG_RANGE_HI);
    jwm.get_paramlist()->add_param(
        synthmodnames::RANGELIMIT, paramnames::SIG_RANGE_LO);
    done_params = true;
}

#endif
