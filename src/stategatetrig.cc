#include "../include/stategatetrig.h"

stategatetrig::stategatetrig(const char* uname) :
 synthmod::base(synthmod::STATEGATETRIG, uname, SM_HAS_OUT_TRIG),
 in_trig(0), in_state(0), out_trig(OFF), out_not_trig(OFF)
{
    register_output(output::OUT_TRIG);
    register_output(output::OUT_NOT_TRIG);
}

void stategatetrig::register_ui()
{
    register_input(input::IN_TRIG);
    register_input(input::IN_STATE);
}

ui::moditem_list* stategatetrig::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

stategatetrig::~stategatetrig()
{
}

const void* stategatetrig::get_out(int ot) const
{
    switch(ot)
    {
        case output::OUT_TRIG:     return &out_trig;
        case output::OUT_NOT_TRIG: return &out_not_trig;
        default: return 0;
    }
}

const void* stategatetrig::set_in(int it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG:   return in_trig = (STATUS*)o;
        case input::IN_STATE:  return in_state = (STATUS*)o;
        default: return 0;
    }
}

const void* stategatetrig::get_in(int it) const
{
    switch(it)
    {
        case input::IN_TRIG:   return in_trig;
        case input::IN_STATE:  return in_state;
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

