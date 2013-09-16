#include "../include/trigcounter.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

trigcounter::trigcounter(char const* uname) :

 synthmod(
    synthmodnames::TRIGCOUNTER,
    uname,
    SM_HAS_OUT_TRIG),

 in_trig(0), in_reset_trig(0),
 out_trig(OFF), out_not_trig(OFF), out_pre_count(0), out_count(-1),
 out_play_state(OFF),
 pre_count(0), count(0), wrap(OFF)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_NOT_TRIG);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_COUNT);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_PRE_COUNT);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE);
    jwm.get_inputlist()->add_input(this, inputnames::IN_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_RESET_TRIG);
    create_params();
}

trigcounter::~trigcounter()
{
}

void const* trigcounter::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_TRIG:         return &out_trig;
        case outputnames::OUT_NOT_TRIG:     return &out_not_trig;
        case outputnames::OUT_PRE_COUNT:    return &out_pre_count;
        case outputnames::OUT_COUNT:        return &out_count;
        case outputnames::OUT_PLAY_STATE:   return &out_play_state;
        default: return 0;
    }
}

void const* trigcounter::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG:      return in_trig = (STATUS*)o;
        case inputnames::IN_RESET_TRIG:return in_reset_trig = (STATUS*)o;
        default: return  0;
    }
}

void const* trigcounter::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG:       return in_trig;
        case inputnames::IN_RESET_TRIG: return in_reset_trig;
        default: return  0;
    }
}

bool trigcounter::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::PRE_COUNT:
            pre_count = *(short*)data;
            return true;
        case paramnames::COUNT:
            count = *(short*)data;
            return true;
        case paramnames::WRAP:
            wrap = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

void const* trigcounter::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::PRE_COUNT: return &pre_count;
        case paramnames::COUNT:     return &count;
        case paramnames::WRAP:      return &wrap;
        default: return 0;
    }
}
#include <iostream>
stockerrs::ERR_TYPE trigcounter::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::PRE_COUNT,
                                            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::PRE_COUNT));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::COUNT,
                                            stockerrs::ERR_ABOVE1))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::COUNT));
        invalidate();
        return stockerrs::ERR_ABOVE1;
    }
    if (count < 1){
        sm_err("PPO %s", "(I'm as confused as you).");
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

void trigcounter::init()
{
    out_pre_count = pre_count;
    count--;
}

void trigcounter::run()
{
    if (*in_reset_trig == ON) {
        out_count = -1;
        if ((out_pre_count = pre_count) > 0)
            out_play_state = OFF;
        else if (count > 0)
            out_play_state = ON;
    }
    if (*in_trig == ON) {
        if (out_pre_count > 0)
            out_pre_count--;
        else if (out_count < count) {
            out_count++;
            out_trig = ON;
            out_play_state = ON;
        }
        else {
            if (wrap == ON) {
                out_count = 0;
                out_trig = ON;
                out_play_state = ON;
            }
            else {
                out_not_trig = ON;
                out_play_state = OFF;
            }
        }
    }
    else {
        if (out_trig == ON)
            out_trig = OFF;
        if (out_not_trig == ON)
            out_not_trig = OFF;
    }
}



void trigcounter::create_params()
{
    if (done_params())
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::TRIGCOUNTER, paramnames::PRE_COUNT);
    jwm.get_paramlist()->add_param(
        synthmodnames::TRIGCOUNTER, paramnames::COUNT);
    jwm.get_paramlist()->add_param(
        synthmodnames::TRIGCOUNTER, paramnames::WRAP);
}

