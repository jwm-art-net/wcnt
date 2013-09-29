#include "../include/modifier.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"

modifier::modifier(const char* uname) :
 synthmod(module::MODIFIER, uname, SM_HAS_OUT_OUTPUT),
 in_signal1(0), in_signal2(0), in_bias(0),
 out_output(0),
 func(ADD), type(M1)
{
    register_input(input::IN_SIGNAL1);
    register_input(input::IN_SIGNAL2);
    register_input(input::IN_BIAS);
    register_output(output::OUT_OUTPUT);
    init_first();
}

modifier::~modifier()
{
}

const void* modifier::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* modifier::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL1: return in_signal1 = (double*)o;
        case input::IN_SIGNAL2: return in_signal2 = (double*)o;
        case input::IN_BIAS:    return in_bias = (double*)o;
        default: return 0;
    }
}

const void* modifier::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL1:     return in_signal1;
        case input::IN_SIGNAL2:     return in_signal2;
        case input::IN_BIAS:        return in_bias;
        default: return 0;
    }
}

bool modifier::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::FUNC:
            func = (FUNC)(*(int*)data);
            return true;
        case param::MODIFIER_TYPE:
            type = (TYPE)(*(int*)data);
            return true;
/*
        case param::BIAS:
            bias = *(double*)data;
            return true;
*/
        default:
            return false;
    }
}

const void* modifier::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::FUNC:          return &func;
        case param::MODIFIER_TYPE: return &type;
//        case param::BIAS:          return &bias;
        default: return 0;
    }
}

stockerrs::ERR_TYPE modifier::validate()
{
/*
    if (!jwm.get_paramlist()->validate(this, param::BIAS,
                                        stockerrs::ERR_RANGE_0_1))
    {
        *err_msg = param::names::get(param::BIAS);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
*/
    return stockerrs::ERR_NO_ERROR;
}

#include <math.h>

void modifier::run()
{
    double bias = fabs(*in_bias);
    if (func == ADD) {
        out_output = *in_signal1 * (1 - bias) + 
            *in_signal2 * fabs(-*in_signal1) * bias *
                (type == M1 ? 1 : *in_signal1);
    }
    else if (func == SUB) {
        out_output = *in_signal1 * (1 - bias) -
            *in_signal2 * fabs(*in_signal1) * bias *
                (type == M1 ? 1 : *in_signal1);
    }
    else {
        out_output = *in_signal1 * (1 - bias) * *in_signal2 +
            *in_signal2 * fabs(*in_signal1) * bias *
                (type == M1 ? 1 : *in_signal1);
    }
}

void modifier::init_first()
{
    if (done_first())
        return;
    register_param(param::FUNC, "add/sub/mul");
    register_param(param::MODIFIER_TYPE, "m1/m2");
/*
    register_param(param::BIAS);
*/
}

