#ifndef MODIFIER_H
#include "../include/modifier.h"

modifier::modifier(char const* uname) :
 synthmod(synthmodnames::MOD_MODIFIER, modifier_count, uname),
 in_signal(0), in_mod(0), out_output(0), func(ADD), bias(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    get_inputlist()->add_input(this, inputnames::IN_MODIFIER);
    modifier_count++;
    create_params();
}

modifier::~modifier()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* modifier::get_out(outputnames::OUT_TYPE ot)
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

void const* modifier::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal = (double*)o;
    case inputnames::IN_MODIFIER:
        return in_mod = (double*)o;
    default:
        return 0;
    }
}

void const* modifier::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal;
    case inputnames::IN_MODIFIER:
        return in_mod;
    default:
        return 0;
    }
}

bool modifier::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_MODIFIER_FUNC:
        set_func((MOD_FUNC)(*(int*)data));
        retv = true;
        break;
    case paramnames::PAR_BIAS:
        set_bias(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* modifier::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_MODIFIER_FUNC:
        return &func;
    case paramnames::PAR_BIAS:
        return &bias;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE modifier::validate()
{
    if (!get_paramlist()->validate(this, paramnames::PAR_BIAS,
                                        stockerrs::ERR_RANGE_0_1))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_BIAS);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    return stockerrs::ERR_NO_ERROR;
}

void modifier::run()
{
    if (func == ADD)
        out_output = *in_signal * (1 - bias) + *in_mod * bias;
    else
        out_output = *in_signal * (1 - bias) - *in_mod * bias;
}

int modifier::modifier_count = 0;

bool modifier::done_params = false;

void modifier::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
            synthmodnames::MOD_MODIFIER, paramnames::PAR_MODIFIER_FUNC);
    get_fxsparamlist()->add_param("add/sub",
                                    paramnames::PAR_MODIFIER_FUNC);
    get_paramlist()->add_param(
            synthmodnames::MOD_MODIFIER, paramnames::PAR_BIAS);
    done_params = true;
}

#endif
