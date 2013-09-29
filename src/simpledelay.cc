#include "../include/simpledelay.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

simple_delay::simple_delay(const char* uname) :
 synthmod(module::SIMPLEDELAY, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), out_output(0), delay_time(0), output(0),
 filter(0), filterarraymax(0), fpos(0), filtertotal(0)
{
    register_input(input::IN_SIGNAL);
    register_output(output::OUT_OUTPUT);
    init_first();
}

simple_delay::~simple_delay()
{
    if (filter)
        delete [] filter;
}

void simple_delay::init()
{
    filterarraymax = (long)((delay_time * jwm.samplerate()) / 1000);
    filter = new double[filterarraymax];
    if (!filter){
        invalidate();
        return;
    }
    for (long i = 0; i < filterarraymax; i++) filter[i] = 0;
    fpos = filterarraymax - 1;
}

const void* simple_delay::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* simple_delay::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal = (double*)o;
        default: return  0;
    }
}

const void* simple_delay::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool simple_delay::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* simple_delay::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::DELAY_TIME: return &delay_time;
        default: return 0;
    }
}

stockerrs::ERR_TYPE simple_delay::validate()
{
    if (!jwm.get_paramlist()->validate(this, param::DELAY_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", param::names::get(
                                            param::DELAY_TIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void simple_delay::run()
{
    out_output = filter[fpos];
    filter[fpos] = *in_signal;
    fpos--;
    if (fpos < 0) fpos = filterarraymax - 1;
}

void simple_delay::init_first()
{
    if (done_first())
        return;
    register_param(param::DELAY_TIME);
}

