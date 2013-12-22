#include "../include/randomtrigger.h"

#include <cstdlib>

randomtrigger::randomtrigger(const char* uname) :
 synthmod::base(synthmod::RANDTRIGGER, uname, SM_HAS_OUT_TRIG),
 in_trig(0), out_trig(OFF), out_not_trig(OFF), probability(0.5),
 not_probability(1.0)
{
    register_output(output::OUT_TRIG);
    register_output(output::OUT_NOT_TRIG);
    srandom(time(0)); //srand(time(0));
}

void randomtrigger::register_ui()
{
    register_input(input::IN_TRIG);
    register_param(param::PROBABILITY);
    register_param(param::NOTPROBABILITY)->set_flags(ui::UI_OPTIONAL);
}

ui::moditem_list* randomtrigger::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

randomtrigger::~randomtrigger()
{
}

const void* randomtrigger::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_TRIG:     return &out_trig;
        case output::OUT_NOT_TRIG: return &out_not_trig;
        default: return 0;
    }
}

const void* randomtrigger::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG: return in_trig = (STATUS*)o;
        default: return 0;
    }
}

const void* randomtrigger::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_TRIG: return in_trig;
        default: return 0;
    }
}

bool randomtrigger::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::PROBABILITY:
            probability = *(double*)data;
            return true;
        case param::NOTPROBABILITY:
            not_probability = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* randomtrigger::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::PROBABILITY:       return &probability;
        case param::NOTPROBABILITY:    return &not_probability;
        default: return 0;
    }
}

errors::TYPE randomtrigger::validate()
{
    if (!validate_param(param::PROBABILITY, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    if (!validate_param(param::NOTPROBABILITY, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    return errors::NO_ERROR;
}

void randomtrigger::run()
{
    if (*in_trig == ON) {
        if (rand() >= RAND_MAX * (1.00 - probability))
            out_trig = ON;
        else if (rand() >= RAND_MAX * (1 - not_probability))
            out_not_trig = ON;
    } else {
        if (out_trig == ON)
            out_trig = OFF;
        if (out_not_trig == ON)
            out_not_trig = OFF;
    }
}

