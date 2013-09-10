#ifndef ECHO_H
#include "../include/echo.h"

echo::echo(char const* uname) :
 synthmod(synthmodnames::MOD_ECHO, echo_count, uname),
 in_signal(0), in_gainmod(0), in_feedback(0), in_feed_mod(0), output(0),
 wet_output(0), delay_time(0), gain(0), gain_modsize(0), feed_level(0),
 feed_modsize(0), wetdry(0), filter(0), filterarraymax(0), fpos(0),
 filtertotal(0), gainamount(0), feedamount(0)
{
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    get_inputlist()->add_input(this, inputnames::IN_GAIN_MOD);
    get_inputlist()->add_input(this, inputnames::IN_FEEDBACK);
    get_inputlist()->add_input(this, inputnames::IN_FB_MOD);
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_outputlist()->add_output(this, outputnames::OUT_WET_OUTPUT);
    echo_count++;
    create_params();
}

echo::~echo()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
    delete [] filter;
}

void echo::init()
{
    filterarraymax = (long)((delay_time * audio_samplerate) / 1000);
    filter = new double[filterarraymax];
    for (long i = 0; i < filterarraymax; i++) filter[i] = 0;
    fpos = filterarraymax - 1;
}

void const* echo::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_OUTPUT:
        o = &output;
        break;
    case outputnames::OUT_WET_OUTPUT:
        o = &wet_output;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* echo::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        i = in_signal = (double*)o;
        break;
    case inputnames::IN_GAIN_MOD:
        i = in_gainmod = (double*)o;
        break;
    case inputnames::IN_FEEDBACK:
        i = in_feedback = (double*)o;
        break;
    case inputnames::IN_FB_MOD:
        i = in_feed_mod = (double*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool echo::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_DELAY_TIME:
        set_delay_time(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_GAIN:
        set_gain_level(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_GAIN_MODSIZE:
        set_gain_modsize(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_FEED_LEVEL:
        set_feedback_level(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_FEED_MODSIZE:
        set_feedback_modsize(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_WETDRY:
        set_wetdry(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* echo::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_DELAY_TIME:
        return &delay_time;
    case paramnames::PAR_GAIN:
        return &gain;
    case paramnames::PAR_GAIN_MODSIZE:
        return &gain_modsize;
    case paramnames::PAR_FEED_LEVEL:
        return &feed_level;
    case paramnames::PAR_FEED_MODSIZE:
        return &feed_modsize;
    case paramnames::PAR_WETDRY:
        return &wetdry;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE echo::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_DELAY_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_DELAY_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::PAR_GAIN,
            stockerrs::ERR_RANGE_M1_1))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_GAIN);
        invalidate();
        return stockerrs::ERR_RANGE_M1_1;
    }
    if (!pl->validate(this, paramnames::PAR_GAIN_MODSIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg
         = get_paramnames()->get_name(paramnames::PAR_GAIN_MODSIZE);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!pl->validate(this, paramnames::PAR_FEED_LEVEL,
            stockerrs::ERR_RANGE_FEED))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_FEED_LEVEL);
        invalidate();
        return stockerrs::ERR_RANGE_FEED;
    }
    if (!pl->validate(this, paramnames::PAR_FEED_MODSIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_FEED_MODSIZE);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!pl->validate(this, paramnames::PAR_WETDRY,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_WETDRY);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    return stockerrs::ERR_NO_ERROR;
}

void echo::run()
{
    gainamount = gain * (1 - gain_modsize)
     + (gain * gain_modsize * *in_gainmod);
    feedamount = feed_level * (1 - feed_modsize)
     + (feed_level * feed_modsize * *in_feed_mod);
    output = *in_signal * gainamount * (1 - wetdry)
     + (wet_output = filter[fpos]) * wetdry;
    filter[fpos] = *in_signal * gainamount + *in_feedback * feedamount;
    fpos--;
    if (fpos < 0) fpos = filterarraymax - 1;
}

int echo::echo_count = 0;

bool echo::done_params = false;

void echo::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_ECHO, paramnames::PAR_DELAY_TIME);
    get_paramlist()->add_param(
     synthmodnames::MOD_ECHO, paramnames::PAR_GAIN);
    get_paramlist()->add_param(
     synthmodnames::MOD_ECHO, paramnames::PAR_GAIN_MODSIZE);
    get_paramlist()->add_param(
     synthmodnames::MOD_ECHO, paramnames::PAR_FEED_LEVEL);
    get_paramlist()->add_param(
     synthmodnames::MOD_ECHO, paramnames::PAR_FEED_MODSIZE);
    get_paramlist()->add_param(
     synthmodnames::MOD_ECHO, paramnames::PAR_WETDRY);
    done_params = true;
}

#endif
