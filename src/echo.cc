#include "../include/echo.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

echo::echo(const char* uname) :
 synthmod(module::ECHO, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), in_gainmod(0), in_feedback(0), in_feed_mod(0), output(0),
 wet_output(0), delay_time(0), gain(0), gain_modsize(0), feed_level(0),
 feed_modsize(0), wetdry(0), filter(0), filterarraymax(0), fpos(0),
 filtertotal(0), gainamount(0), feedamount(0)
{
    register_input(input::IN_SIGNAL);
    register_input(input::IN_GAIN_MOD);
    register_input(input::IN_FEEDBACK);
    register_input(input::IN_FB_MOD);
    register_output(output::OUT_OUTPUT);
    register_output(output::OUT_WET_OUTPUT);
    init_first();
}

echo::~echo()
{
    delete [] filter;
}

const void* echo::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT:       return &output;
        case output::OUT_WET_OUTPUT:   return &wet_output;
        default: return 0;
    }
}

const void* echo::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL:     return in_signal = (double*)o;
        case input::IN_GAIN_MOD:   return in_gainmod = (double*)o;
        case input::IN_FEEDBACK:   return in_feedback = (double*)o;
        case input::IN_FB_MOD:     return in_feed_mod = (double*)o;
        default: return 0;
    }
}

const void* echo::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL:     return in_signal;
        case input::IN_GAIN_MOD:   return in_gainmod;
        case input::IN_FEEDBACK:   return in_feedback;
        case input::IN_FB_MOD:     return in_feed_mod;
        default: return 0;
    }
}

bool echo::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        case param::GAIN:
            gain = *(double*)data;
            return true;
        case param::GAIN_MODSIZE:
            gain_modsize = *(double*)data;
            return true;
        case param::FEED_LEVEL:
            feed_level = *(double*)data;
            return true;
        case param::FEED_MODSIZE:
            feed_modsize = *(double*)data;
            return true;
        case param::WETDRY:
            wetdry = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* echo::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::DELAY_TIME:    return &delay_time;
        case param::GAIN:          return &gain;
        case param::GAIN_MODSIZE:  return &gain_modsize;
        case param::FEED_LEVEL:    return &feed_level;
        case param::FEED_MODSIZE:  return &feed_modsize;
        case param::WETDRY:        return &wetdry;
        default: return 0;
    }
}

stockerrs::ERR_TYPE echo::validate()
{
    modparamlist* pl = jwm.get_paramlist();
    if (!pl->validate(this, param::DELAY_TIME,
                                            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", param::names::get(
                                            param::DELAY_TIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, param::GAIN,
                                            stockerrs::ERR_RANGE_M1_1))
    {
        sm_err("%s", param::names::get(param::GAIN));
        invalidate();
        return stockerrs::ERR_RANGE_M1_1;
    }
    if (!pl->validate(this, param::GAIN_MODSIZE,
                                            stockerrs::ERR_RANGE_0_1))
    {
        sm_err("%s", param::names::get(
                                            param::GAIN_MODSIZE));
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!pl->validate(this, param::FEED_LEVEL,
                                            stockerrs::ERR_RANGE_FEED))
    {
        sm_err("%s", param::names::get(
                                            param::FEED_LEVEL));
        invalidate();
        return stockerrs::ERR_RANGE_FEED;
    }
    if (!pl->validate(this, param::FEED_MODSIZE,
                                            stockerrs::ERR_RANGE_0_1))
    {
        sm_err("%s", param::names::get(
                                            param::FEED_MODSIZE));
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!pl->validate(this, param::WETDRY,
            stockerrs::ERR_RANGE_0_1))
    {
        sm_err("%s", param::names::get(param::WETDRY));
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
    register_param(param::DELAY_TIME);
    register_param(param::GAIN);
    register_param(param::GAIN_MODSIZE);
    register_param(param::FEED_LEVEL);
    register_param(param::FEED_MODSIZE);
    register_param(param::WETDRY);
}

