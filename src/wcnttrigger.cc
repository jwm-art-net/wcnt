#ifndef WCNTTRIGGER_H
#include "../include/wcnttrigger.h"

wcnt_trigger::wcnt_trigger(char const* uname) :
 synthmod(synthmodnames::MOD_WCNTTRIGGER, wcnt_trigger_count, uname),
 in_trig(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_TRIG);
    wcnt_trigger_count++;
}

wcnt_trigger::~wcnt_trigger()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* wcnt_trigger::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_TRIG:
        o = in_trig;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* wcnt_trigger::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_TRIG:
        i = in_trig = (STATUS*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

int wcnt_trigger::wcnt_trigger_count = 0;

#endif
