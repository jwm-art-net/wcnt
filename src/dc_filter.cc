#include "../include/dc_filter.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/dtr.h"

dc_filter::dc_filter(const char* uname) :
 synthmod(module::DCFILTER, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), output(0), dc_time(0), filter(0), filterarraymax(0),
 fpos(0), filtertotal(0)
{
    register_input(input::IN_SIGNAL);
    register_output(output::OUT_OUTPUT);
    init_first();
}

dc_filter::~dc_filter()
{
    delete [] filter;
}

const void* dc_filter::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

const void* dc_filter::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

const void* dc_filter::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool dc_filter::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::DC_TIME:
            dc_time = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* dc_filter::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::DC_TIME: return &dc_time;
        default: return 0;
    }
}

stockerrs::ERR_TYPE dc_filter::validate()
{
    if (!jwm.get_paramlist()->validate(this, param::DC_TIME,
            stockerrs::ERR_NEG_ZERO))
    {
        sm_err("%s", param::names::get(param::DC_TIME));
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    return stockerrs::ERR_NO_ERROR;
}

void dc_filter::init()
{
    filterarraymax = (short)((dc_time * jwm.samplerate()) / 1000);
    filter = new double[filterarraymax];
    if (!filter){
        invalidate();
        return;
    }
    for (int i = 0; i < filterarraymax; i++) filter[i] = 0;
}

void dc_filter::run()
{
    output = (filtertotal / filterarraymax) - filter[fpos];
    filtertotal -= filter[fpos];
    filtertotal += (filter[fpos] = *in_signal);
    fpos++;
    if (fpos == filterarraymax)
        fpos = 0;
}

void dc_filter::init_first()
{
    if (done_first())
        return;
    register_param(param::DC_TIME);
}

