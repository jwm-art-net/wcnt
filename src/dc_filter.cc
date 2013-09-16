#include "../include/dc_filter.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/dtr.h"

dc_filter::dc_filter(char const* uname) :
 synthmod(synthmodnames::DCFILTER, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), output(0), dc_time(0), filter(0), filterarraymax(0),
 fpos(0), filtertotal(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    create_params();
}

dc_filter::~dc_filter()
{
    delete [] filter;
}

void const* dc_filter::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

void const* dc_filter::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

void const* dc_filter::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool dc_filter::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::DC_TIME:
            dc_time = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* dc_filter::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::DC_TIME: return &dc_time;
        default: return 0;
    }
}

stockerrs::ERR_TYPE dc_filter::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::DC_TIME,
            stockerrs::ERR_NEG_ZERO))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::DC_TIME);
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

void dc_filter::create_params()
{
    if (done_params())
        return;
    jwm.get_paramlist()->add_param(
     synthmodnames::DCFILTER, paramnames::DC_TIME);
}

