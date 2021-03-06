#include "../include/trigger.h"
#include "../include/conversions.h"

#include <math.h>

trigger::trigger(const char* uname) :
 synthmod::base(synthmod::TRIGGER, uname, SM_HAS_OUT_TRIG),
 in_signal(0), out_trig(OFF), out_not_trig(OFF), out_wait_state(OFF),
 delay_time(0.0), trigger_level(0.0), delay_samps(0)
{
    register_output(output::OUT_TRIG);
    register_output(output::OUT_WAIT_STATE);
}

void trigger::register_ui()
{
    register_input(input::IN_SIGNAL);
    register_param(param::DELAY_TIME);
    register_param(param::TRIGGER_LEVEL);
}

ui::moditem_list* trigger::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

trigger::~trigger()
{
}

const void* trigger::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_TRIG:         return &out_trig;
        case output::OUT_NOT_TRIG:     return &out_not_trig;
        case output::OUT_WAIT_STATE:   return &out_wait_state;
        default: return 0;
    }
}

const void* trigger::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

const void* trigger::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool trigger::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        case param::TRIGGER_LEVEL:
            trigger_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* trigger::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::DELAY_TIME:    return &delay_time;
        case param::TRIGGER_LEVEL: return &trigger_level;
        default: return 0;
    }
}

errors::TYPE trigger::validate()
{
    if (!validate_param(param::DELAY_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::TRIGGER_LEVEL, errors::NEG_OR_ZERO))
        return errors::NEG_OR_ZERO;

    return errors::NO_ERROR;
}

void trigger::run()
{
    double fs = fabs(*in_signal);
    if (out_wait_state == OFF) {
        if (fs > trigger_level) {
            out_trig = ON;
            out_wait_state = ON;
            delay_samps = ms_to_samples(delay_time);
        }
        if (out_not_trig == ON)
            out_not_trig = OFF;
    }
    else {
        if (delay_samps == 0) {
            if (fs < trigger_level) {
                out_not_trig = ON;
                out_wait_state = OFF;
            }
        }
        else
            delay_samps--;
        if (out_trig == ON)
            out_trig = OFF;
    }
}
