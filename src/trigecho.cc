#include "../include/trigecho.h"
#include "../include/jwm_globals.h"
#include "../include/conversions.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

trigecho::trigecho(char const* uname) :

 synthmod(
    synthmodnames::TRIGECHO,
    uname,
    SM_HAS_OUT_TRIG),

 in_trig(0), out_trig(OFF), out_velocity(0), out_count(0),
 delay_time(0.0), count(0), send_input_out(OFF),
 past_trigs(0), pastmax(0), pastpos(0), vel_count_ratio(0)
{
    register_input(inputnames::IN_TRIG);
    register_output(outputnames::OUT_TRIG);
    register_output(outputnames::OUT_COUNT);
    register_output(outputnames::OUT_VELOCITY);
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

void const* trigecho::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_TRIG:     return &out_trig;
        case outputnames::OUT_COUNT:    return &out_count;
        case outputnames::OUT_VELOCITY: return &out_velocity;
        default: return 0;
    }
}

void const* trigecho::set_in(inputnames::IN_TYPE it, void const* o)
{
    if (it == inputnames::IN_TRIG) {
        in_trig = (STATUS*)o;
        return o;
    }
    return  0;
}

void const* trigecho::get_in(inputnames::IN_TYPE it) const
{
    if (it == inputnames::IN_TRIG)
        return in_trig;
    return  0;
}

bool trigecho::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        case paramnames::COUNT:
            count = *(short*)data;
            return true;
        case paramnames::SEND_INPUT_OUT:
            send_input_out = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

void const* trigecho::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::DELAY_TIME:        return &delay_time;
        case paramnames::COUNT:             return &count;
        case paramnames::SEND_INPUT_OUT:    return &send_input_out;
        default: return 0;
    }
}

stockerrs::ERR_TYPE trigecho::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::DELAY_TIME,
                                            stockerrs::ERR_NEG_ZERO))
    {
        sm_err("%s", paramnames::get_name(
                                            paramnames::DELAY_TIME));
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::COUNT,
                                            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", paramnames::get_name(paramnames::COUNT));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
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
    register_param(paramnames::DELAY_TIME);
    register_param(paramnames::COUNT);
    register_param(paramnames::SEND_INPUT_OUT);
}

