#include "../include/delay.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

delay::delay(const char* uname) :
 synthmod(module::DELAY, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), in_gainmod(0), out_output(0), delay_time(0), gain(0),
 gain_modsize(0), wetdry(0), output(0), filter(0), filterarraymax(0),
 fpos(0), filtertotal(0), gainamount(0)
{
    register_input(input::IN_SIGNAL);
    register_input(input::IN_GAIN_MOD);
    register_output(output::OUT_OUTPUT);
    init_first();
}

delay::~delay()
{
    if (filter)
        delete [] filter;
}

const void* delay::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* delay::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL:     return in_signal = (double*)o;
        case input::IN_GAIN_MOD:   return in_gainmod = (double*)o;
        default: return  0;
    }
}

const void* delay::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL:     return in_signal;
        case input::IN_GAIN_MOD:   return in_gainmod;
        default: return 0;
    }
}

bool delay::set_param(param::TYPE pt, const void* data)
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
        case param::WETDRY:
            wetdry = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* delay::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::DELAY_TIME:    return &delay_time;
        case param::GAIN:          return &gain;
        case param::GAIN_MODSIZE:  return &gain_modsize;
        case param::WETDRY:        return &wetdry;
        default: return 0;
    }
}

errors::TYPE delay::validate()
{
    if (!validate_param(param::DELAY_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::GAIN, errors::RANGE_M1_1))
        return errors::RANGE_M1_1;

    if (!validate_param(param::GAIN_MODSIZE, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    if (!validate_param(param::WETDRY, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    return errors::NO_ERROR;
}

void delay::init()
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

void delay::run()
{
    #ifdef DUPLICATE_BRANCHES
    if (*in_gainmod >= 0)
        gainamount = gain - gain * gain_modsize
                     + (gain * gain_modsize * *in_gainmod);
    else
    #endif
    gainamount = gain - gain * gain_modsize
                     + (gain * gain_modsize * *in_gainmod);

    output = filter[fpos];
    filter[fpos] = *in_signal * gainamount;
    fpos--;
    if (fpos < 0) fpos = filterarraymax - 1;
    out_output = output * wetdry + *in_signal * (1 - wetdry);
}

void delay::init_first()
{
    if (done_first())
        return;
    register_param(param::DELAY_TIME);
    register_param(param::GAIN);
    register_param(param::GAIN_MODSIZE);
    register_param(param::WETDRY);
}

