#ifndef DELAY_H
#include "../include/rms.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

#include <math.h>

rms::rms(char const* uname) :
 synthmod(synthmodnames::RMS, uname),
 in_signal(0), out_rms(0), rms_time(0), output(0), rmsarr(0),
 arraymax(0), arrpos(0), sqrsum(0)
{
    jwm.get_outputlist().add_output(this, outputnames::OUT_RMS);
    jwm.get_inputlist().add_input(this, inputnames::IN_SIGNAL);
    create_params();
}

#include <iostream>

rms::~rms()
{
    jwm.get_outputlist().delete_module_outputs(this);
    jwm.get_inputlist().delete_module_inputs(this);
    if (rmsarr)
        delete [] rmsarr;
    else
        std::cout << "\noh shit!";
}

void rms::init()
{
    arraymax = (short)((rms_time * jwm.samplerate()) / 1000);
    rmsarr = new double[arraymax];
    for (int i = 0; i < arraymax; i++)
        rmsarr[i] = 0;
}

void const* rms::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_RMS: return &out_rms;
        default: return 0;
    }
}

void const* rms::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

void const* rms::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool rms::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::RMS_TIME:
            rms_time = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* rms::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::RMS_TIME: return &rms_time;
        default: return 0;
    }
}

stockerrs::ERR_TYPE rms::validate()
{
    if (!jwm.get_paramlist().validate(this, paramnames::RMS_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = jwm.get_paramnames().get_name(paramnames::RMS_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void rms::run()
{
// really should have worked this one out, myself:
    sqrsum -= rmsarr[arrpos];
    sqrsum += (rmsarr[arrpos] = *in_signal * *in_signal);
    arrpos++;
    if (arrpos == arraymax)
        arrpos = 0;
    out_rms = sqrt(sqrsum / arraymax);
}

/*
    sqrsum = 0;
    for (int i = arrpos; i < arrpos + arraymax; i++)
        sqrsum += rmsarr[i % arraymax];
    out_rms = sqrt(sqrsum / arraymax);
    rmsarr[arrpos] = *in_signal * *in_signal;
    arrpos--;
    if (arrpos < 0)
        arrpos = arraymax - 1;
*/


bool rms::done_params = false;

void rms::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist().add_param(synthmodnames::RMS,
        paramnames::RMS_TIME);
    done_params = true;
}

#endif
