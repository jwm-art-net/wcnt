#ifndef WCNTSIGNAL_H
#include "../include/wcntsignal.h"

wcnt_signal::wcnt_signal(char const* uname) :
 synthmod(synthmodnames::MOD_WCNTSIGNAL, wcnt_signal_count, uname),
 in_signal(0), out_output(0.0), level(0.0)
{
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    create_params();
    wcnt_signal_count++;
}

wcnt_signal::~wcnt_signal()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* wcnt_signal::get_out(outputnames::OUT_TYPE ot)
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

void const* wcnt_signal::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        i = in_signal = (double*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool wcnt_signal::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_LEVEL:
        set_level(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* wcnt_signal::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_LEVEL:
        return &level;
    default:
        return 0;
    }
}

bool wcnt_signal::done_params = false;

void wcnt_signal::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_WCNTSIGNAL, paramnames::PAR_LEVEL);
    done_params = true;
}

int wcnt_signal::wcnt_signal_count = 0;

#endif
