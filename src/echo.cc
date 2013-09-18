#include "../include/echo.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

echo::echo(char const* uname) :
 synthmod(synthmodnames::ECHO, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), in_gainmod(0), in_feedback(0), in_feed_mod(0), output(0),
 wet_output(0), delay_time(0), gain(0), gain_modsize(0), feed_level(0),
 feed_modsize(0), wetdry(0), filter(0), filterarraymax(0), fpos(0),
 filtertotal(0), gainamount(0), feedamount(0)
{
    register_input(inputnames::IN_SIGNAL);
    register_input(inputnames::IN_GAIN_MOD);
    register_input(inputnames::IN_FEEDBACK);
    register_input(inputnames::IN_FB_MOD);
    register_output(outputnames::OUT_OUTPUT);
    register_output(outputnames::OUT_WET_OUTPUT);
    init_first();
}

echo::~echo()
{
    delete [] filter;
}

void const* echo::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT:       return &output;
        case outputnames::OUT_WET_OUTPUT:   return &wet_output;
        default: return 0;
    }
}

void const* echo::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:     return in_signal = (double*)o;
        case inputnames::IN_GAIN_MOD:   return in_gainmod = (double*)o;
        case inputnames::IN_FEEDBACK:   return in_feedback = (double*)o;
        case inputnames::IN_FB_MOD:     return in_feed_mod = (double*)o;
        default: return 0;
    }
}

void const* echo::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:     return in_signal;
        case inputnames::IN_GAIN_MOD:   return in_gainmod;
        case inputnames::IN_FEEDBACK:   return in_feedback;
        case inputnames::IN_FB_MOD:     return in_feed_mod;
        default: return 0;
    }
}

bool echo::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        case paramnames::GAIN:
            gain = *(double*)data;
            return true;
        case paramnames::GAIN_MODSIZE:
            gain_modsize = *(double*)data;
            return true;
        case paramnames::FEED_LEVEL:
            feed_level = *(double*)data;
            return true;
        case paramnames::FEED_MODSIZE:
            feed_modsize = *(double*)data;
            return true;
        case paramnames::WETDRY:
            wetdry = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* echo::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::DELAY_TIME:    return &delay_time;
        case paramnames::GAIN:          return &gain;
        case paramnames::GAIN_MODSIZE:  return &gain_modsize;
        case paramnames::FEED_LEVEL:    return &feed_level;
        case paramnames::FEED_MODSIZE:  return &feed_modsize;
        case paramnames::WETDRY:        return &wetdry;
        default: return 0;
    }
}

stockerrs::ERR_TYPE echo::validate()
{
    modparamlist* pl = jwm.get_paramlist();
    if (!pl->validate(this, paramnames::DELAY_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::DELAY_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::GAIN,
            stockerrs::ERR_RANGE_M1_1))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::GAIN);
        invalidate();
        return stockerrs::ERR_RANGE_M1_1;
    }
    if (!pl->validate(this, paramnames::GAIN_MODSIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg
         = jwm.get_paramnames()->get_name(paramnames::GAIN_MODSIZE);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!pl->validate(this, paramnames::FEED_LEVEL,
            stockerrs::ERR_RANGE_FEED))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::FEED_LEVEL);
        invalidate();
        return stockerrs::ERR_RANGE_FEED;
    }
    if (!pl->validate(this, paramnames::FEED_MODSIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::FEED_MODSIZE);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!pl->validate(this, paramnames::WETDRY,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::WETDRY);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    return stockerrs::ERR_NO_ERROR;
}

void echo::init()
{
    filterarraymax = (long)((delay_time * jwm.samplerate()) / 1000);
    filter = new double[filterarraymax];
    if (!filter){
        invalidate();
        return;
    }
    for (long i = 0; i < filterarraymax; i++) filter[i] = 0;
    fpos = filterarraymax - 1;
    return;
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

void echo::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::DELAY_TIME);
    register_param(paramnames::GAIN);
    register_param(paramnames::GAIN_MODSIZE);
    register_param(paramnames::FEED_LEVEL);
    register_param(paramnames::FEED_MODSIZE);
    register_param(paramnames::WETDRY);
}

