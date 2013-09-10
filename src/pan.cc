#ifndef PAN_H
#include "../include/pan.h"

pan::pan(char const* uname) :
 synthmod(synthmodnames::MOD_PAN, pan_count,	uname),
 in_signal(0), in_pan_mod(0), out_l(0), out_r(0), panpos(0),
 pan_modsize(0), pan_pos(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_L);
    get_outputlist()->add_output(this, outputnames::OUT_R);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    get_inputlist()->add_input(this, inputnames::IN_PAN_MOD);
    pan_count++;
    create_params();
}

pan::~pan()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* pan::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_L:
        o = &out_l;
        break;
    case outputnames::OUT_R:
        o = &out_r;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* pan::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        i = in_signal = (double*)o;
        break;
    case inputnames::IN_PAN_MOD:
        i = in_pan_mod = (double*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool pan::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_PAN:
        set_pan(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_PAN_MODSIZE:
        set_pan_modsize(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* pan::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_PAN:
        return &pan_pos;
    case paramnames::PAR_PAN_MODSIZE:
        return &pan_modsize;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE pan::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_PAN,
            stockerrs::ERR_RANGE_M1_1))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_PAN);
        invalidate();
        return stockerrs::ERR_RANGE_M1_1;
    }
    if (!pl->validate(this, paramnames::PAR_PAN_MODSIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_PAN_MODSIZE);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    return stockerrs::ERR_NO_ERROR;
}

void pan::run()
{
    if ((pan_mod = *in_pan_mod) > 1) pan_mod = 1;
    else if (pan_mod < -1) pan_mod = -1;
//    pan_pos = panpos * (1 - pan_modsize) + pan_mod * pan_modsize;
    pan_pos = panpos + pan_modsize * pan_mod;
    if (pan_pos > 0) {
        out_l = *in_signal;
        out_r = *in_signal * (1 - pan_pos);
    } else {
        out_l = *in_signal * (1 + pan_pos);
        out_r = *in_signal;
    }
}

int pan::pan_count = 0;

bool pan::done_params = false;

void pan::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_PAN, paramnames::PAR_PAN);
    get_paramlist()->add_param(
     synthmodnames::MOD_PAN, paramnames::PAR_PAN_MODSIZE);
    done_params = true;
}
#endif
