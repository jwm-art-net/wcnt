#include "../include/trigecho.h"
#include "../include/jwm_globals.h"
#include "../include/conversions.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

trigecho::trigecho(const char* uname) :
 synthmod(module::TRIGECHO, uname, SM_HAS_OUT_TRIG),
 in_trig(0), out_trig(OFF), out_velocity(0), out_count(0),
 delay_time(0.0), count(0), send_input_out(OFF),
 past_trigs(0), pastmax(0), pastpos(0), vel_count_ratio(0)
{
    register_input(input::IN_TRIG);
    register_output(output::OUT_TRIG);
    register_output(output::OUT_COUNT);
    register_output(output::OUT_VELOCITY);
    init_first();
}

trigecho::~trigecho()
{
    if (past_trigs)
        delete [] past_trigs;
}

void trigecho::init()
{
    pastmax = ms_to_samples(delay_time);
    past_trigs = new short[pastmax];
    if(!past_trigs){
        invalidate();
        return;
    }
    for (unsigned long i = 0; i < pastmax; i++) past_trigs[i] = 0;
    pastpos = pastmax - 1;
    vel_count_ratio = 1.0 / (double)count;
}

const void* trigecho::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_TRIG:     return &out_trig;
        case output::OUT_COUNT:    return &out_count;
        case output::OUT_VELOCITY: return &out_velocity;
        default: return 0;
    }
}

const void* trigecho::set_in(input::TYPE it, const void* o)
{
    return (it == input::IN_TRIG ? (in_trig = (STATUS*)o) : 0);
}

const void* trigecho::get_in(input::TYPE it) const
{
    return (it == input::IN_TRIG ? in_trig : 0);
}

bool trigecho::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        case param::COUNT:
            count = *(short*)data;
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

    if (!validate_param(param::COUNT, errors::NEGATIVE))
        return errors::NEGATIVE;

    return errors::NO_ERROR;
}

void trigecho::run()
{
    if (past_trigs[pastpos] > 0) {
        out_trig = ON;
        past_trigs[pastpos]--;
        out_velocity = past_trigs[pastpos] * vel_count_ratio;
        out_count = count - past_trigs[pastpos];
    }
    else if (out_trig == ON)
        out_trig = OFF;
    if (*in_trig == ON) {
        past_trigs[pastpos] = count;
        if (send_input_out == ON) {
            out_velocity = 1.0;
            out_count = 1;
            out_trig = ON;
        }
    }
    pastpos++;
    if (pastpos == pastmax) pastpos = 0;
}

void trigecho::init_first()
{
    if (done_first())
        return;
    register_param(param::DELAY_TIME);
    register_param(param::COUNT);
    register_param(param::SEND_INPUT_OUT);
}

