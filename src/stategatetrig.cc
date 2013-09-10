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
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_TRIG:
        o = &out_trig;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* stategatetrig::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_TRIG:
        i = in_trig = (STATUS*)o;
        break;
    case inputnames::IN_STATE:
        i = in_state = (STATUS*)o;
        break;
    default:
        i = 0;
    }
    return i;
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
