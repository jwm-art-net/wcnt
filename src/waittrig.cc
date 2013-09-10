#ifndef WAITTRIG_H
#include "../include/waittrig.h"

waittrig::waittrig(char const* uname) :
 synthmod(synthmodnames::MOD_WAITTRIG, waittrig_count, uname),
 in_trig1(0), in_trig2(0), out_trig(OFF), out_wait_state(ON),
 min_time(0), max_time(0), count(1), min_samples(0), max_samples(0),
 mins(0), maxs(0), counter(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_WAIT_STATE);
    get_inputlist()->add_input(this, inputnames::IN_TRIG1);
    get_inputlist()->add_input(this, inputnames::IN_TRIG2);
    waittrig_count++;
    create_params();
}

waittrig::~waittrig()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void waittrig::set_min_time(double t)
{
    min_time = t;
    mins = min_samples = ms_to_samples(min_time);
}

void waittrig::set_max_time(double t)
{
    max_time = t;
    maxs = max_samples = ms_to_samples(max_time);
}

void const* waittrig::get_out(outputnames::OUT_TYPE ot)
{
    switch(ot)
    {
    case outputnames::OUT_TRIG:
        return &out_trig;
    case outputnames::OUT_WAIT_STATE:
        return &out_wait_state;
    default:
        return 0;
    }
}

void const* waittrig::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_TRIG1:
        return in_trig1 = (STATUS*)o;
    case inputnames::IN_TRIG2:
        return in_trig2 = (STATUS*)o;
    default:
        return 0;
    }
}

void const* waittrig::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_TRIG1:
        return in_trig1;
    case inputnames::IN_TRIG2:
        return in_trig2;
    default:
        return 0;
    }
}

bool waittrig::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
    case paramnames::PAR_MIN_WAIT:
        set_min_time(*(double*)data);
        return true;
    case paramnames::PAR_MAX_WAIT:
        set_max_time(*(double*)data);
        return true;
    case paramnames::PAR_COUNT:
        set_count(*(short*)data);
        return true;
    default:
        return false;
    }
}

void const* waittrig::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_MIN_WAIT:
        return &min_time;
    case paramnames::PAR_MAX_WAIT:
        return &max_time;
    case paramnames::PAR_COUNT:
        return &count;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE waittrig::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_MIN_WAIT,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_MIN_WAIT);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::PAR_MAX_WAIT,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_MAX_WAIT);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::PAR_COUNT,
            stockerrs::ERR_NEG_ZERO))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_COUNT);
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    return stockerrs::ERR_NO_ERROR;
}

void waittrig::init()
{
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

int waittrig::waittrig_count = 0;

bool waittrig::done_params = false;

void waittrig::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_WAITTRIG, paramnames::PAR_MIN_WAIT);
    get_paramlist()->add_param(
     synthmodnames::MOD_WAITTRIG, paramnames::PAR_MAX_WAIT);
    get_paramlist()->add_param(
     synthmodnames::MOD_WAITTRIG, paramnames::PAR_COUNT);
    done_params = true;
}

#endif
