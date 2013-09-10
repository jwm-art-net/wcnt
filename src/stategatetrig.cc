#ifndef STATEGATETRIG_H
#include "../include/stategatetrig.h"

stategatetrig::stategatetrig(char const* uname) :
 synthmod(synthmodnames::MOD_STATEGATETRIG, stategatetrig_count, uname),
 in_trig(0), in_state(0), out_trig(OFF)
{
    get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_STATE);
    stategatetrig_count++;
}

stategatetrig::~stategatetrig()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* stategatetrig::get_out(outputnames::OUT_TYPE ot)
{
    switch(ot)
    {
    case outputnames::OUT_TRIG:
        return &out_trig;
    default:
        return 0;
    }
}

void const* stategatetrig::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_TRIG:
        return in_trig = (STATUS*)o;
    case inputnames::IN_STATE:
        return in_state = (STATUS*)o;
    default:
        return 0;
    }
}

void const* stategatetrig::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_TRIG:
        return in_trig;
    case inputnames::IN_STATE:
        return in_state;
    default:
        return 0;
    }
}

void stategatetrig::run()
{
    if (*in_state == ON) {
        if (*in_trig == ON) 
            out_trig = ON;
        else if (out_trig == ON)
            out_trig = OFF;
    }
    else if (out_trig == ON)
        out_trig = OFF;
}

int stategatetrig::stategatetrig_count = 0;

#endif
