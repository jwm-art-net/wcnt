#include "../include/modifier.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"

modifier::modifier(char const* uname) :
 synthmod(synthmodnames::MODIFIER, uname, SM_HAS_OUT_OUTPUT),
 in_signal1(0), in_signal2(0), in_bias(0),
 out_output(0),
 func(ADD), type(M1)
{
    add_output(outputnames::OUT_OUTPUT);
    add_input(inputnames::IN_SIGNAL1);
    add_input(inputnames::IN_SIGNAL2);
    add_input(inputnames::IN_BIAS);
    create_params();
}

modifier::~modifier()
{
}

void const* modifier::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* modifier::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL1: return in_signal1 = (double*)o;
        case inputnames::IN_SIGNAL2: return in_signal2 = (double*)o;
        case inputnames::IN_BIAS:    return in_bias = (double*)o;
        default: return 0;
    }
}

void const* modifier::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL1:     return in_signal1;
        case inputnames::IN_SIGNAL2:     return in_signal2;
        case inputnames::IN_BIAS:        return in_bias;
        default: return 0;
    }
}

bool modifier::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::FUNC:
            func = (FUNC)(*(int*)data);
            return true;
        case paramnames::MODIFIER_TYPE:
            type = (TYPE)(*(int*)data);
            return true;
/*
        case paramnames::BIAS:
            bias = *(double*)data;
            return true;
*/
        default:
            return false;
    }
}

void const* modifier::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::FUNC:          return &func;
        case paramnames::MODIFIER_TYPE: return &type;
//        case paramnames::BIAS:          return &bias;
        default: return 0;
    }
}

stockerrs::ERR_TYPE modifier::validate()
{
/*
    if (!jwm.get_paramlist()->validate(this, paramnames::BIAS,
                                        stockerrs::ERR_RANGE_0_1))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::BIAS);
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

void modifier::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::FUNC, "add/sub/mul");
    relate_param(paramnames::MODIFIER_TYPE, "m1/m2");
/*
    relate_param(paramnames::BIAS);
*/
}

