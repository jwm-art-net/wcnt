#include "../include/trigcounter.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

trigcounter::trigcounter(const char* uname) :
 synthmod(module::TRIGCOUNTER, uname, SM_HAS_OUT_TRIG),
 in_trig(0), in_reset_trig(0),
 out_trig(OFF), out_not_trig(OFF), out_pre_count(0), out_count(-1),
 out_play_state(OFF),
 pre_count(0), count(0), wrap(OFF)
{
    register_input(input::IN_TRIG);
    register_input(input::IN_RESET_TRIG);
    register_output(output::OUT_TRIG);
    register_output(output::OUT_NOT_TRIG);
    register_output(output::OUT_COUNT);
    register_output(output::OUT_PRE_COUNT);
    register_output(output::OUT_PLAY_STATE);
    init_first();
}

trigcounter::~trigcounter()
{
}

const void* trigcounter::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_TRIG:         return &out_trig;
        case output::OUT_NOT_TRIG:     return &out_not_trig;
        case output::OUT_PRE_COUNT:    return &out_pre_count;
        case output::OUT_COUNT:        return &out_count;
        case output::OUT_PLAY_STATE:   return &out_play_state;
        default: return 0;
    }
}

const void* trigcounter::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG:      return in_trig = (STATUS*)o;
        case input::IN_RESET_TRIG:return in_reset_trig = (STATUS*)o;
        default: return  0;
    }
}

const void* trigcounter::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_TRIG:       return in_trig;
        case input::IN_RESET_TRIG: return in_reset_trig;
        default: return  0;
    }
}

bool trigcounter::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::PRE_COUNT:
            pre_count = *(short*)data;
            return true;
        case param::COUNT:
            count = *(short*)data;
            return true;
        case param::WRAP:
            wrap = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

const void* trigcounter::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::PRE_COUNT: return &pre_count;
        case param::COUNT:     return &count;
        case param::WRAP:      return &wrap;
        default: return 0;
    }
}
#include <iostream>
stockerrs::ERR_TYPE trigcounter::validate()
{
    if (!jwm.get_paramlist()->validate(this, param::PRE_COUNT,
                                            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", param::names::get(param::PRE_COUNT));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_paramlist()->validate(this, param::COUNT,
                                            stockerrs::ERR_ABOVE1))
    {
        sm_err("%s", param::names::get(param::COUNT));
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



void trigcounter::init_first()
{
    if (done_first())
        return;
    register_param(param::PRE_COUNT);
    register_param(param::COUNT);
    register_param(param::WRAP);
}

