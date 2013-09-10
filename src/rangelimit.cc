#ifndef RANGELIMIT_H
#include "../include/rangelimit.h"

range_limit::range_limit(char const* uname) :
 synthmod(synthmodnames::MOD_RANGELIMIT, range_limit_count, uname),
 in_signal(0), out_output(0), sigrangehi(0), sigrangelo(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    range_limit_count++;
    create_params();
}

range_limit::~range_limit()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* range_limit::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_OUTPUT:
        o = &out_output;
        break;
    default:
        o = 0;
    }
    return o;
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

void const* range_limit::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal;
    default:
        return 0;
    }
}

bool range_limit::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_SIG_RANGE_HI:
        set_signal_range_hi(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_SIG_RANGE_LO:
        set_signal_range_lo(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* range_limit::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_SIG_RANGE_HI:
        return &sigrangehi;
    case paramnames::PAR_SIG_RANGE_LO:
        return &sigrangelo;
    default:
        return 0;
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

int range_limit::range_limit_count = 0;

bool range_limit::done_params = false;

void range_limit::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_RANGELIMIT, paramnames::PAR_SIG_RANGE_HI);
    get_paramlist()->add_param(
     synthmodnames::MOD_RANGELIMIT, paramnames::PAR_SIG_RANGE_LO);
    done_params = true;
}

#endif
