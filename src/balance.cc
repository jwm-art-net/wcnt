#ifndef BALANCE_H
#include "../include/balance.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"

balance::balance(char const* uname) :
 synthmod(synthmodnames::BALANCE, uname),
 in_signal1(0), in_signal2(0),out_output(0), func(ADD), bias(0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist().add_input(this, inputnames::IN_SIGNAL1);
    jwm.get_inputlist().add_input(this, inputnames::IN_SIGNAL2);
    create_params();
}

balance::~balance()
{
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
}

void const* balance::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* balance::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL1:     return in_signal1 = (double*)o;
        case inputnames::IN_SIGNAL2:     return in_signal2 = (double*)o;
        default: return 0;
    }
}

void const* balance::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL1:     return in_signal1;
        case inputnames::IN_SIGNAL2:     return in_signal2;
        default: return 0;
    }
}

bool balance::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::FUNC:
            func = (FUNC)(*(int*)data);
            return true;
        case paramnames::BIAS:
            bias = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* balance::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::FUNC: return &func;
        case paramnames::BIAS: return &bias;
        default: return 0;
    }
}

stockerrs::ERR_TYPE balance::validate()
{
    if (!jwm.get_paramlist().validate(this, paramnames::BIAS,
                                        stockerrs::ERR_RANGE_0_1))
    {
        *err_msg = jwm.get_paramnames().get_name(paramnames::BIAS);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    return stockerrs::ERR_NO_ERROR;
}

void balance::run()
{
    if (func == ADD)
        out_output = *in_signal1 * (1 - bias) + *in_signal2 * bias;
    else if (func == SUB)
        out_output = *in_signal1 * (1 - bias) - *in_signal2 * bias;
    else
        out_output = *in_signal1 * (1 - bias)
            + *in_signal2 * bias * *in_signal1;
}

bool balance::done_params = false;

void balance::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist().add_param(
        synthmodnames::BALANCE, paramnames::FUNC);
    jwm.get_fxsparamlist().add_param("add/sub/mul",
        paramnames::FUNC);
    jwm.get_paramlist().add_param(
        synthmodnames::BALANCE, paramnames::BIAS);
    done_params = true;
}

#endif
