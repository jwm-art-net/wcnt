#include "../include/balance.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"

balance::balance(const char* uname) :
 synthmod(module::BALANCE, uname, SM_HAS_OUT_OUTPUT),
 in_signal1(0), in_signal2(0),out_output(0), func(ADD), bias(0)
{
    register_input(input::IN_SIGNAL1);
    register_input(input::IN_SIGNAL2);
    register_output(output::OUT_OUTPUT);
    init_first();
}

balance::~balance()
{
}

const void* balance::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* balance::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL1:     return in_signal1 = (double*)o;
        case input::IN_SIGNAL2:     return in_signal2 = (double*)o;
        default: return 0;
    }
}

const void* balance::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL1:     return in_signal1;
        case input::IN_SIGNAL2:     return in_signal2;
        default: return 0;
    }
}

bool balance::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::FUNC:
            func = (FUNC)(*(int*)data);
            return true;
        case param::BIAS:
            bias = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* balance::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::FUNC: return &func;
        case param::BIAS: return &bias;
        default: return 0;
    }
}

stockerrs::ERR_TYPE balance::validate()
{
    if (!jwm.get_paramlist()->validate(this, param::BIAS,
                                        stockerrs::ERR_RANGE_0_1))
    {
        sm_err("%s", param::names::get(param::BIAS));
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

void balance::init_first()
{
    if (done_first())
        return;
    register_param(param::FUNC, "add/sub/mul");
    register_param(param::BIAS);
}

