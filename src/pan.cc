#include "../include/pan.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

pan::pan(char const* uname) :
 synthmod(synthmodnames::PAN, uname, SM_HAS_STEREO_OUTPUT),
 in_signal(0), in_pan_mod(0), out_l(0), out_r(0), panpos(0),
 pan_modsize(0), pan_pos(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_LEFT);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_RIGHT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    jwm.get_inputlist()->add_input(this, inputnames::IN_PAN_MOD);
    create_params();
}

pan::~pan()
{
}

void const* pan::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_LEFT: return &out_l;
        case outputnames::OUT_RIGHT:return &out_r;
        default: return 0;
    }
}

void const* pan::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal = (double*)o;
        case inputnames::IN_PAN_MOD:return in_pan_mod = (double*)o;
        default: return 0;
    }
}

void const* pan::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        case inputnames::IN_PAN_MOD:return in_pan_mod;
        default: return 0;
    }
}

bool pan::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::PAN:
            panpos = *(double*)data;
            return true;
        case paramnames::PAN_MODSIZE:
            pan_modsize = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* pan::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::PAN:        return &pan_pos;
        case paramnames::PAN_MODSIZE:return &pan_modsize;
        default: return 0;
    }
}

stockerrs::ERR_TYPE pan::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::PAN,
            stockerrs::ERR_RANGE_M1_1))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::PAN));
        invalidate();
        return stockerrs::ERR_RANGE_M1_1;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::PAN_MODSIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        sm_err("%s", 
                jwm.get_paramnames()->get_name(paramnames::PAN_MODSIZE));
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

bool pan::done_params = false;

void pan::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(synthmodnames::PAN, paramnames::PAN);
    jwm.get_paramlist()->add_param(synthmodnames::PAN,
                                                paramnames::PAN_MODSIZE);
    done_params = true;
}
