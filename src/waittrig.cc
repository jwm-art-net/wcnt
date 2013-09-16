#include "../include/waittrig.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

waittrig::waittrig(char const* uname) :

 synthmod(
    synthmodnames::WAITTRIG,
    uname,
    SM_HAS_OUT_TRIG),

 in_trig1(0), in_trig2(0), out_trig(OFF), out_wait_state(ON),
 min_time(0), max_time(0), count(1), min_samples(0), max_samples(0),
 mins(0), maxs(0), counter(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_WAIT_STATE);
    jwm.get_inputlist()->add_input(this, inputnames::IN_TRIG1);
    jwm.get_inputlist()->add_input(this, inputnames::IN_TRIG2);
    create_params();
}

waittrig::~waittrig()
{
}

void const* waittrig::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_TRIG:         return &out_trig;
        case outputnames::OUT_WAIT_STATE:   return &out_wait_state;
        default: return 0;
    }
}

void const* waittrig::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG1: return in_trig1 = (STATUS*)o;
        case inputnames::IN_TRIG2: return in_trig2 = (STATUS*)o;
        default: return 0;
    }
}

void const* waittrig::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG1: return in_trig1;
        case inputnames::IN_TRIG2: return in_trig2;
        default: return 0;
    }
}

bool waittrig::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::MIN_WAIT:
            min_time = *(double*)data;
            return true;
        case paramnames::MAX_WAIT:
            max_time = *(double*)data;
            return true;
        case paramnames::COUNT:
            count = *(short*)data;
            return true;
        default:
            return false;
    }
}

void const* waittrig::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::MIN_WAIT:  return &min_time;
        case paramnames::MAX_WAIT:  return &max_time;
        case paramnames::COUNT:     return &count;
        default: return 0;
    }
}

stockerrs::ERR_TYPE waittrig::validate()
{
    modparamlist* pl = jwm.get_paramlist();
    if (!pl->validate(this, paramnames::MIN_WAIT,
            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::MIN_WAIT));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::MAX_WAIT,
            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::MAX_WAIT));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::COUNT,
            stockerrs::ERR_NEG_ZERO))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::COUNT));
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    return stockerrs::ERR_NO_ERROR;
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



void waittrig::create_params()
{
    if (done_params())
        return;
    jwm.get_paramlist()->add_param(
     synthmodnames::WAITTRIG, paramnames::MIN_WAIT);
    jwm.get_paramlist()->add_param(
     synthmodnames::WAITTRIG, paramnames::MAX_WAIT);
    jwm.get_paramlist()->add_param(
     synthmodnames::WAITTRIG, paramnames::COUNT);
    
}

