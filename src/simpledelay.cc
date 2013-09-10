#ifndef SIMPLEDELAY_H
#include "../include/simpledelay.h"

simple_delay::simple_delay(char const* uname) :
 synthmod(synthmodnames::MOD_SIMPLEDELAY, simple_delay_count, uname),
 in_signal(0), out_output(0), delay_time(0), output(0),
 filter(0), filterarraymax(0), fpos(0), filtertotal(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    simple_delay_count++;
    create_params();
}

simple_delay::~simple_delay()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
    if (filter)
        delete [] filter;
}

void simple_delay::init()
{
    filterarraymax = (long)((delay_time * audio_samplerate) / 1000);
    filter = new double[filterarraymax];
    for (long i = 0; i < filterarraymax; i++) filter[i] = 0;
    fpos = filterarraymax - 1;
}

void const* simple_delay::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_OUTPUT:
        o = &out_output;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* simple_delay::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal = (double*)o;
    default:
        return  0;
    }
}

void const* simple_delay::get_in(inputnames::IN_TYPE it)
{
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        return in_signal;
    default:
        return 0;
    }
}

bool simple_delay::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
    case paramnames::PAR_DELAY_TIME:
        set_delay_time(*(double*)data);
        return true;
    default:
        return false;
    }
}

void const* simple_delay::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_DELAY_TIME:
        return &delay_time;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE simple_delay::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_DELAY_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_DELAY_TIME);
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

int simple_delay::simple_delay_count = 0;

bool simple_delay::done_params = false;

void simple_delay::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_SIMPLEDELAY, paramnames::PAR_DELAY_TIME);
    done_params = true;
}

#endif
