#ifndef MODIFIER_H
#include "../include/modifier.h"

modifier::modifier(char const* uname) :
 synthmod(synthmodnames::MOD_MODIFIER, modifier_count, uname),
 in_signal(0), in_mod(0), out_output(0.00), modifier_func(ADD),
 mod_size(1.00), min_out_val(0.00), max_out_val(1.00), posnegmirror(OFF),
 type_func(0)
{
#ifndef BARE_MODULES
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    get_inputlist()->add_input(this, inputnames::IN_MODIFIER);
#endif
    modifier_count++;
#ifndef BARE_MODULES
    create_params();
#endif
}

modifier::~modifier()
{
#ifndef BARE_MODULES
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
#endif
}

#ifndef BARE_MODULES
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
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        i = in_signal = (double*)o;
        break;
    case inputnames::IN_MODIFIER:
        i = in_mod = (double*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool modifier::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_MODIFIER_FUNC:
        set_modifier_func(*(MOD_FUNC*)data);
        retv = true;
        break;
    case paramnames::PAR_MODIFIER_SIZE:
        set_modifier_size(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_MIN_OUT:
        set_min_output_value(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_MAX_OUT:
        set_max_output_value(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_POSNEG_MIRROR:
        set_posneg_mirror(*(STATUS*)data);
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
        return &modifier_func;
    case paramnames::PAR_MODIFIER_SIZE:
        return &mod_size;
    case paramnames::PAR_MIN_OUT:
        return &min_out_val;
    case paramnames::PAR_MAX_OUT:
        return &max_out_val;
    case paramnames::PAR_POSNEG_MIRROR:
        return &posnegmirror;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE modifier::validate()
{
    // ...to be decided...
    return stockerrs::ERR_NO_ERROR;
}

#endif

void modifier::init()
{
    negmin = (min_out_val < 0) ? min_out_val : -min_out_val;
    posmin = -negmin;
    negmax = (max_out_val < 0) ? max_out_val : -max_out_val;
    posmax = -negmax;
    switch(modifier_func)
    {
    case ADD:
        type_func = &modifier::func_add;
        return;
    case SUB:
        type_func = &modifier::func_sub;
        return;
    case MUL:
        type_func = &modifier::func_mul;
        return;
    case DIV:
        type_func = &modifier::func_div;
        return;
    case MOD:
        type_func = &modifier::func_mod;
        return;
    case SIN:
        type_func = &modifier::func_sin;
        return;
    case COS:
        type_func = &modifier::func_cos;
        return;
    case TAN:
        type_func = &modifier::func_tan;
        return;
    case AND:
        type_func = &modifier::func_and;
        return;
    case OR:
        type_func = &modifier::func_or;
        return;
    case XOR:
        type_func = &modifier::func_xor;
        return;
    default:
        type_func = &modifier::func_add;
    }
}

void modifier::run()
{
    tmp_out = (this->*type_func)();
    if (posnegmirror == ON)
    {
        if (tmp_out < 0)
        {
            if (tmp_out > negmin)
                out_output = negmin;
            else if (tmp_out < negmax)
                out_output = negmax;
            else
                out_output = tmp_out;
        }
        else
        {
            if (tmp_out < posmin)
                out_output = posmin;
            else if (tmp_out > posmax)
                out_output = posmax;
            else
                out_output = tmp_out;
        }
    }
    else
    {
        if (tmp_out < min_out_val)
            out_output = min_out_val;
        else if (tmp_out > max_out_val)
            out_output = max_out_val;
        else
            out_output = tmp_out;
    }
}

int modifier::modifier_count = 0;

#ifndef BARE_MODULES
bool modifier::done_params = false;

void modifier::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_MODIFIER, paramnames::PAR_MODIFIER_FUNC);
    get_paramlist()->add_param(
     synthmodnames::MOD_MODIFIER, paramnames::PAR_MODIFIER_SIZE);
    get_paramlist()->add_param(
     synthmodnames::MOD_MODIFIER, paramnames::PAR_MIN_OUT);
    get_paramlist()->add_param(
     synthmodnames::MOD_MODIFIER, paramnames::PAR_MAX_OUT);
    get_paramlist()->add_param(
     synthmodnames::MOD_MODIFIER, paramnames::PAR_POSNEG_MIRROR);
    done_params = true;
}
#endif
#endif
