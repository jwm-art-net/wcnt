#include "../include/trigecho.h"
#include "../include/conversions.h"

trigecho::trigecho(const char* uname) :
 synthmod::base(synthmod::TRIGECHO, uname, SM_HAS_OUT_TRIG),
 in_trig(0), in_reset_trig(0),
 out_trig(OFF), out_velocity(0), out_index(0),
 delay_time(0.0), count(0), send_input_out(OFF),
 past_trigs(0), pastmax(0), pastpos(0), vel_count_ratio(0), last(0)
{
    register_output(output::OUT_TRIG);
    register_output(output::OUT_INDEX);
    register_output(output::OUT_VELOCITY);
}

void trigecho::register_ui()
{
    register_param(param::COUNT);
    register_param(param::DELAY_TIME);
    register_input(input::IN_RESET_TRIG);
    register_param(param::SEND_INPUT_OUT);
    register_input(input::IN_TRIG);
}

ui::moditem_list* trigecho::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

trigecho::~trigecho()
{
    if (past_trigs)
        delete [] past_trigs;
}

void trigecho::init()
{
    pastmax = ms_to_samples(delay_time);
    past_trigs = new wcint_t[pastmax];
    if(!past_trigs){
        invalidate();
        return;
    }
    for (int i = 0; i < pastmax; i++) past_trigs[i] = 0;
    pastpos = pastmax - 1;
    vel_count_ratio = 1.0 / (double)count;
    last = count + (send_input_out == ON ? 1 : 0);
}

const void* trigecho::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_TRIG:     return &out_trig;
        case output::OUT_INDEX:    return &out_index;
        case output::OUT_VELOCITY: return &out_velocity;
        default: return 0;
    }
}

const void* trigecho::set_in(input::TYPE it, const void* o)
{
    switch(it) {
    case input::IN_TRIG:        return in_trig = (STATUS*)o;
    case input::IN_RESET_TRIG:  return in_reset_trig = (STATUS*)o;
    default:
        return 0;
    }
}

const void* trigecho::get_in(input::TYPE it) const
{
    switch(it) {
    case input::IN_TRIG:        return in_trig;
    case input::IN_RESET_TRIG:  return in_reset_trig;
    default:
        return 0;
    }
}

bool trigecho::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        case param::COUNT:
            count = *(wcint_t*)data;
            return true;
        case param::SEND_INPUT_OUT:
            send_input_out = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

const void* trigecho::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::DELAY_TIME:        return &delay_time;
        case param::COUNT:             return &count;
        case param::SEND_INPUT_OUT:    return &send_input_out;
        default: return 0;
    }
}

errors::TYPE trigecho::validate()
{
    if (!validate_param(param::DELAY_TIME, errors::NEG_OR_ZERO))
        return errors::NEG_OR_ZERO;

    if (!validate_param(param::COUNT, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    return errors::NO_ERROR;
}

void trigecho::run()
{
    if (*in_reset_trig == ON) {
        out_index = -1;
        out_trig = OFF;
        out_velocity = 0.0f;
        for (int i = 0; i < pastmax; ++i)
            past_trigs[i] = 0;
        return;
    }

    if (*in_trig == ON) {
        past_trigs[pastpos] = last;
        if (send_input_out == ON) {
            out_velocity = 1.0;
            out_index = 0;
            out_trig = ON;
        }
    }
    else if (past_trigs[pastpos] > 0) {
        out_trig = ON;
        past_trigs[pastpos]--;
        out_velocity = past_trigs[pastpos] * vel_count_ratio;
        out_index = last - 1 - past_trigs[pastpos];
    }
    else if (out_trig == ON)
        out_trig = OFF;

    if (++pastpos == pastmax)
        pastpos = 0;
}

