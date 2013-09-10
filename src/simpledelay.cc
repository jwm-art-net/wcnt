#ifndef SIMPLEDELAY_H
#include "../include/simpledelay.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

simple_delay::simple_delay(char const* uname) :
 synthmod(synthmodnames::SIMPLEDELAY, uname),
 in_signal(0), out_output(0), delay_time(0), output(0),
 filter(0), filterarraymax(0), fpos(0), filtertotal(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    create_params();
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

void const* simple_delay::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* simple_delay::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal = (double*)o;
        default: return  0;
    }
}

void const* simple_delay::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool simple_delay::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* simple_delay::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::DELAY_TIME: return &delay_time;
        default: return 0;
    }
}

stockerrs::ERR_TYPE simple_delay::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::DELAY_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::DELAY_TIME);
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

bool simple_delay::done_params = false;

void simple_delay::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::SIMPLEDELAY, paramnames::DELAY_TIME);
    done_params = true;
}

#endif
