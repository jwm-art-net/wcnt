#include "../include/rms.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

#include <math.h>

rms::rms(const char* uname) :
 synthmod(module::RMS, uname, SM_DEFAULT),
 in_signal(0), out_rms(0), rms_time(0), output(0), rmsarr(0),
 arraymax(0), arrpos(0), sqrsum(0)
{
    register_input(input::IN_SIGNAL);
    register_output(output::OUT_RMS);
    init_first();
}

rms::~rms()
{
    if (rmsarr)
        delete [] rmsarr;
}

const void* rms::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_RMS: return &out_rms;
        default: return 0;
    }
}

const void* rms::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

const void* rms::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool rms::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::RMS_TIME:
            rms_time = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* rms::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::RMS_TIME: return &rms_time;
        default: return 0;
    }
}

errors::TYPE rms::validate()
{
    if (!validate_param(param::RMS_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    return errors::NO_ERROR;
}

void rms::init()
{
    arraymax = (wcint_t)((rms_time * jwm.samplerate()) / 1000);
    rmsarr = new double[arraymax];
    if (!rmsarr) {
        invalidate();
        return;
    }
    for (int i = 0; i < arraymax; i++)
        rmsarr[i] = 0;
}

void rms::run()
{
    sqrsum -= rmsarr[arrpos];
    sqrsum += (rmsarr[arrpos] = *in_signal * *in_signal);
    arrpos++;
    if (arrpos == arraymax)
        arrpos = 0;
    out_rms = sqrt(sqrsum / arraymax);
}

void rms::init_first()
{
    if (done_first())
        return;
    register_param(param::RMS_TIME);
}

