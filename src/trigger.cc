#include "../include/trigger.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

#include <math.h>

trigger::trigger(char const* uname) :

 synthmod(
    synthmodnames::TRIGGER,
    uname,
    SM_HAS_OUT_TRIG),

 in_signal(0), out_trig(OFF), out_not_trig(OFF), out_wait_state(OFF),
 delay_time(0.0), trigger_level(0.0), delay_samps(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_WAIT_STATE);
    jwm.get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    create_params();
}

trigger::~trigger()
{
}

void const* trigger::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_TRIG:         return &out_trig;
        case outputnames::OUT_NOT_TRIG:     return &out_not_trig;
        case outputnames::OUT_WAIT_STATE:   return &out_wait_state;
        default: return 0;
    }
}

void const* trigger::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

void const* trigger::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool trigger::set_param(paramnames::PAR_TYPE pt, void const* data)
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

void const* trigger::get_param(paramnames::PAR_TYPE pt) const
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
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::DELAY_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::TRIGGER_LEVEL,
            stockerrs::ERR_NEG_ZERO))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::TRIGGER_LEVEL);
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

bool trigger::done_params = false;

void trigger::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(
     synthmodnames::TRIGGER, paramnames::DELAY_TIME);
    jwm.get_paramlist()->add_param(
     synthmodnames::TRIGGER, paramnames::TRIGGER_LEVEL);
    done_params = true;
}
