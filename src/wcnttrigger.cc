#ifndef WCNTTRIGGER_H
#include "../include/wcnttrigger.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"

wcnt_trigger::wcnt_trigger(char const* uname) :
 synthmod(synthmodnames::WCNTTRIGGER, uname),
 in_trig(0)
{
    set_empty_run();
    jwm.get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_TRIG);
}

wcnt_trigger::~wcnt_trigger()
{
}

void const* wcnt_trigger::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_TRIG: return in_trig;
        default: return 0;
    }
}

void const* wcnt_trigger::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig = (STATUS*)o;
        default: return 0;
    }
}

void const* wcnt_trigger::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig;
        default: return 0;
    }
}

#endif
