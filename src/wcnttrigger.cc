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
    switch(ot)
    {
    case outputnames::OUT_TRIG:
        return in_trig;
    default:
        return 0;
    }
}

void const* wcnt_trigger::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_TRIG:
        return in_trig = (STATUS*)o;
    default:
        return 0;
    }
}

void const* wcnt_trigger::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_TRIG:
        return in_trig;
    default:
        return 0;
    }
}

int wcnt_trigger::wcnt_trigger_count = 0;

#endif
