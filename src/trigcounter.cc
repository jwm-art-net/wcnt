#include "../include/trigcounter.h"

trigcounter::trigcounter(const char* uname) :
 synthmod::base(synthmod::TRIGCOUNTER, uname, SM_HAS_OUT_TRIG),
 in_trig(0), in_reset_trig(0),
 out_trig(OFF), out_not_trig(OFF), out_pre_count(-1), out_count(-1),
 out_play_state(OFF),
 pre_count(0), count(0), wrap(OFF)
{
    register_output(output::OUT_TRIG);
    register_output(output::OUT_NOT_TRIG);
    register_output(output::OUT_COUNT);
    register_output(output::OUT_PRE_COUNT);
    register_output(output::OUT_PLAY_STATE);
}

void trigcounter::register_ui()
{
    register_input(input::IN_TRIG);
    register_param(param::PRE_COUNT)->set_flags(ui::UI_GROUP1);
    register_param(param::COUNT)->set_flags(ui::UI_GROUP1)->add_descr("The number of triggers to count (and output), 0 for unlimited.");
    register_param(param::WRAP)->set_flags(ui::UI_GROUP1);
    register_input(input::IN_RESET_TRIG)->set_flags(ui::UI_GROUP1);
}

ui::moditem_list* trigcounter::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

trigcounter::~trigcounter()
{
}

const void* trigcounter::get_out(int ot) const
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

const void* trigcounter::set_in(int it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG:      return in_trig = (STATUS*)o;
        case input::IN_RESET_TRIG:return in_reset_trig = (STATUS*)o;
        default: return  0;
    }
}

const void* trigcounter::get_in(int it) const
{
    switch(it)
    {
        case input::IN_TRIG:       return in_trig;
        case input::IN_RESET_TRIG: return in_reset_trig;
        default: return  0;
    }
}

bool trigcounter::set_param(int pt, const void* data)
{
    switch(pt)
    {
        case param::PRE_COUNT:
            pre_count = *(wcint_t*)data;
            return true;
        case param::COUNT:
            count = *(wcint_t*)data;
            return true;
        case param::WRAP:
            wrap = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

const void* trigcounter::get_param(int pt) const
{
    switch(pt)
    {
        case param::PRE_COUNT: return &pre_count;
        case param::COUNT:     return &count;
        case param::WRAP:      return &wrap;
        default: return 0;
    }
}

errors::TYPE trigcounter::validate()
{
    if (!validate_param(param::PRE_COUNT, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    if (!validate_param(param::COUNT, errors::RANGE_COUNT1))
        return errors::RANGE_COUNT1;
    return errors::NO_ERROR;
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

