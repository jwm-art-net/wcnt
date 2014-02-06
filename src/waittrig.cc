#include "../include/waittrig.h"
#include "../include/conversions.h"

waittrig::waittrig(const char* uname) :
 synthmod::base(synthmod::WAITTRIG, uname, SM_HAS_OUT_TRIG),
 in_trig1(0), in_trig2(0), out_trig(OFF), out_wait_state(ON),
 min_time(0), max_time(0), count(1), min_samples(0), max_samples(0),
 mins(0), maxs(0), counter(0)
{
    register_output(output::OUT_TRIG);
    register_output(output::OUT_WAIT_STATE);
}

void waittrig::register_ui()
{
    register_input(input::IN_TRIG1);
    register_input(input::IN_TRIG2);
    register_param(param::MIN_WAIT);
    register_param(param::MAX_WAIT);
    register_param(param::COUNT);
}

ui::moditem_list* waittrig::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

waittrig::~waittrig()
{
}

const void* waittrig::get_out(int ot) const
{
    switch(ot)
    {
        case output::OUT_TRIG:         return &out_trig;
        case output::OUT_WAIT_STATE:   return &out_wait_state;
        default: return 0;
    }
}

const void* waittrig::set_in(int it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG1: return in_trig1 = (STATUS*)o;
        case input::IN_TRIG2: return in_trig2 = (STATUS*)o;
        default: return 0;
    }
}

const void* waittrig::get_in(int it) const
{
    switch(it)
    {
        case input::IN_TRIG1: return in_trig1;
        case input::IN_TRIG2: return in_trig2;
        default: return 0;
    }
}

bool waittrig::set_param(int pt, const void* data)
{
    switch(pt)
    {
        case param::MIN_WAIT:
            min_time = *(double*)data;
            return true;
        case param::MAX_WAIT:
            max_time = *(double*)data;
            return true;
        case param::COUNT:
            count = *(wcint_t*)data;
            return true;
        default:
            return false;
    }
}

const void* waittrig::get_param(int pt) const
{
    switch(pt)
    {
        case param::MIN_WAIT:  return &min_time;
        case param::MAX_WAIT:  return &max_time;
        case param::COUNT:     return &count;
        default: return 0;
    }
}

errors::TYPE waittrig::validate()
{
    if (!validate_param(param::MIN_WAIT, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::MAX_WAIT, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::COUNT, errors::RANGE_COUNT1))
        return errors::RANGE_COUNT1;

    return errors::NO_ERROR;
}

void waittrig::init()
{
    mins = min_samples = ms_to_samples(min_time);
    maxs = max_samples = ms_to_samples(max_time);
    mins = min_samples;
    maxs = max_samples;
    out_wait_state = ON;
}

void waittrig::run()
{
    if (out_wait_state == ON) {
        if (mins == 0) {
            if (*in_trig2 == ON || (maxs == 0 && max_samples > 0))
            {
                out_wait_state = OFF;
                counter = count;
            }
        }
        else if (min_samples > 0)
            mins--;
        if (max_samples > 0)
            maxs--;
    }
    if (out_wait_state == OFF) {
        if (*in_trig1 == ON) {
            out_trig = ON;
            counter--;
            if (counter == 0) {
                mins = min_samples;
                maxs = max_samples;
                out_wait_state = ON;
            }
        }
        else if (out_trig == ON)
            out_trig = OFF;
    }
    else if (out_trig == ON)
        out_trig = OFF;
}


