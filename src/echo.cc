#include "../include/echo.h"
#include "../include/globals.h"

echo::echo(const char* uname) :
 synthmod::base(synthmod::ECHO, uname, SM_HAS_OUT_OUTPUT),
 gain(this),
 in_signal(0), in_feedback(0), in_feed_mod(0), output(0),
 wet_output(0), delay_time(0), feed_level(0),
 feed_modsize(0), wetdry(0), filter(0), filterarraymax(0), fpos(0),
 filtertotal(0), feedamount(0)
{
    register_output(output::OUT_OUTPUT);
    register_output(output::OUT_WET_OUTPUT);
}

void echo::register_ui()
{
    register_input(input::IN_FEEDBACK);
    register_input(input::IN_FB_MOD);
    register_param(param::DELAY_TIME);
    register_param(param::FEED_LEVEL);
    register_param(param::FEED_MODSIZE);
    register_param(param::WETDRY);
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
        case input::IN_FEEDBACK:   return in_feedback = (double*)o;
        case input::IN_FB_MOD:     return in_feed_mod = (double*)o;
        default:
            return gain::set_in(it, o);
    }
}

const void* echo::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_FEEDBACK:   return in_feedback;
        case input::IN_FB_MOD:     return in_feed_mod;
        default:
            return gain::get_in(it);
    }
}

bool echo::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::DELAY_TIME:
            delay_time = *(double*)data;
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
            return gain::set_param(pt, data);
    }
}

const void* echo::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::DELAY_TIME:    return &delay_time;
        case param::FEED_LEVEL:    return &feed_level;
        case param::FEED_MODSIZE:  return &feed_modsize;
        case param::WETDRY:        return &wetdry;
        default:
            return gain::get_param(pt);
    }
}

errors::TYPE echo::validate()
{
    if (!validate_param(param::DELAY_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::FEED_LEVEL, errors::RANGE_FEED))
        return errors::RANGE_FEED;

    if (!validate_param(param::FEED_MODSIZE, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    if (!validate_param(param::WETDRY, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    return gain::validate();
}

void echo::init()
{
    filterarraymax = (long)((delay_time * wcnt::jwm.samplerate()) / 1000);
    filter = new double[filterarraymax];
    if (!filter){
        invalidate();
        return;
    }
    for (long i = 0; i < filterarraymax; i++)
        filter[i] = 0;
    fpos = filterarraymax - 1;

    gain::init();
}

void echo::run()
{
    gain::run();
    feedamount = feed_level * (1 - feed_modsize)
                        + (feed_level * feed_modsize * *in_feed_mod);
    output = gain::out * (1 - wetdry)
                        + (wet_output = filter[fpos]) * wetdry;
    filter[fpos] = gain::out + *in_feedback * feedamount;
    if (--fpos < 0)
        fpos = filterarraymax - 1;
}

