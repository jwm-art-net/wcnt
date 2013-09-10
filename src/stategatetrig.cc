#ifndef STATEGATETRIG_H
#include "../include/stategatetrig.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"

stategatetrig::stategatetrig(char const* uname) :

 synthmod(
    synthmodnames::STATEGATETRIG,
    uname,
    SM_HAS_OUT_TRIG),

 in_trig(0), in_state(0), out_trig(OFF), out_not_trig(OFF)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_NOT_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_STATE);
}

stategatetrig::~stategatetrig()
{
}

void const* stategatetrig::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_TRIG:     return &out_trig;
        case outputnames::OUT_NOT_TRIG: return &out_not_trig;
        default: return 0;
    }
}

void const* stategatetrig::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG:   return in_trig = (STATUS*)o;
        case inputnames::IN_STATE:  return in_state = (STATUS*)o;
        default: return 0;
    }
}

void const* stategatetrig::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG:   return in_trig;
        case inputnames::IN_STATE:  return in_state;
        default: return 0;
    }
}

void stategatetrig::run()
{
    if (*in_trig == ON) {
        if (*in_state == ON)
            out_trig = ON;
        else
            out_not_trig = ON;
    }
    else if (out_trig == ON)
        out_trig = OFF;
    else if (out_not_trig == ON)
        out_not_trig = OFF;
}

#endif
