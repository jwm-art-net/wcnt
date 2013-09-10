#ifndef TRIGGER_H
#include "../include/trigger.h"

trigger::trigger(char const* uname) :
 synthmod(synthmodnames::MOD_TRIGGER, trigger_count, uname),
 in_signal(0), out_trig(OFF), out_not_trig(OFF), out_wait_state(OFF),
 delay_time(0.0), trigger_level(0.0), delay_samps(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_WAIT_STATE);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    trigger_count++;
    create_params();
}

trigger::~trigger()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* trigger::get_out(outputnames::OUT_TYPE ot)
{
    switch(ot)
    {
    case outputnames::OUT_TRIG:
        return &out_trig;
    case outputnames::OUT_NOT_TRIG:
        return &out_not_trig;
    case outputnames::OUT_WAIT_STATE:
        return &out_wait_state;
    default:
        return 0;
    }
}

void const* trigger::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal = (double*)o;
        break;
    default:
        return 0;
    }
}

void const* trigger::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal;
    default:
        return 0;
    }
}

bool trigger::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
    case paramnames::PAR_DELAY_TIME:
        set_delay_time(*(double*)data);
        return true;
    case paramnames::PAR_TRIGGER_LEVEL:
        set_trigger_level(*(double*)data);
        return true;
    default:
        return false;
    }
}

void const* trigger::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_DELAY_TIME:
        return &delay_time;
    case paramnames::PAR_TRIGGER_LEVEL:
        return &trigger_level;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE trigger::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_DELAY_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_DELAY_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::PAR_TRIGGER_LEVEL,
            stockerrs::ERR_NEG_ZERO))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_TRIGGER_LEVEL);
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
        if (delay_samps < 0)
            cout << "fucking shit on me fokker!";
        if (out_trig == ON)
            out_trig = OFF;
    }
}

int trigger::trigger_count = 0;

bool trigger::done_params = false;

void trigger::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_TRIGGER, paramnames::PAR_DELAY_TIME);
    get_paramlist()->add_param(
     synthmodnames::MOD_TRIGGER, paramnames::PAR_TRIGGER_LEVEL);
    done_params = true;
}
#endif
