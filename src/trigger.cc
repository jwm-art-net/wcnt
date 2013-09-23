#include "../include/trigger.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

#include <math.h>

trigger::trigger(const char* uname) :

 synthmod(
    synthmodnames::TRIGGER,
    uname,
    SM_HAS_OUT_TRIG),

 in_signal(0), out_trig(OFF), out_not_trig(OFF), out_wait_state(OFF),
 delay_time(0.0), trigger_level(0.0), delay_samps(0)
{
    register_input(inputnames::IN_SIGNAL);
    register_output(outputnames::OUT_TRIG);
    register_output(outputnames::OUT_WAIT_STATE);
    init_first();
}

trigger::~trigger()
{
}

const void* trigger::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_TRIG:         return &out_trig;
        case outputnames::OUT_NOT_TRIG:     return &out_not_trig;
        case outputnames::OUT_WAIT_STATE:   return &out_wait_state;
        default: return 0;
    }
}

const void* trigger::set_in(inputnames::IN_TYPE it, const void* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

const void* trigger::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool trigger::set_param(paramnames::PAR_TYPE pt, const void* data)
{
    switch(pt)
    {
        case paramnames::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        case paramnames::TRIGGER_LEVEL:
            trigger_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* trigger::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::DELAY_TIME:    return &delay_time;
        case paramnames::TRIGGER_LEVEL: return &trigger_level;
        default: return 0;
    }
}

stockerrs::ERR_TYPE trigger::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::DELAY_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s",
                paramnames::get_name(paramnames::DELAY_TIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::TRIGGER_LEVEL,
            stockerrs::ERR_NEG_ZERO))
    {
        sm_err("%s",
             paramnames::get_name(paramnames::TRIGGER_LEVEL));
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    return stockerrs::ERR_NO_ERROR;
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

void trigger::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::DELAY_TIME);
    register_param(paramnames::TRIGGER_LEVEL);
}
