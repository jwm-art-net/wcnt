#include "../include/delay.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

delay::delay(char const* uname) :
 synthmod(synthmodnames::DELAY, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), in_gainmod(0), out_output(0), delay_time(0), gain(0),
 gain_modsize(0), wetdry(0), output(0), filter(0), filterarraymax(0),
 fpos(0), filtertotal(0), gainamount(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    jwm.get_inputlist()->add_input(this, inputnames::IN_GAIN_MOD);
    create_params();
}

delay::~delay()
{
    if (filter)
        delete [] filter;
}

void const* delay::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* delay::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:     return in_signal = (double*)o;
        case inputnames::IN_GAIN_MOD:   return in_gainmod = (double*)o;
        default: return  0;
    }
}

void const* delay::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:     return in_signal;
        case inputnames::IN_GAIN_MOD:   return in_gainmod;
        default: return 0;
    }
}

bool delay::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        case paramnames::GAIN:
            gain = *(double*)data;
            return true;
        case paramnames::GAIN_MODSIZE:
            gain_modsize = *(double*)data;
            return true;
        case paramnames::WETDRY:
            wetdry = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* delay::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::DELAY_TIME:    return &delay_time;
        case paramnames::GAIN:          return &gain;
        case paramnames::GAIN_MODSIZE:  return &gain_modsize;
        case paramnames::WETDRY:        return &wetdry;
        default: return 0;
    }
}

stockerrs::ERR_TYPE delay::validate()
{
    modparamlist* pl = jwm.get_paramlist();
    if (!pl->validate(this, paramnames::DELAY_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(
                                            paramnames::DELAY_TIME));
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::GAIN,
            stockerrs::ERR_RANGE_M1_1))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::GAIN));
        invalidate();
        return stockerrs::ERR_RANGE_M1_1;
    }
    if (!pl->validate(this, paramnames::GAIN_MODSIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(
                                            paramnames::GAIN_MODSIZE));
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!pl->validate(this, paramnames::WETDRY,
            stockerrs::ERR_RANGE_0_1))
    {
        sm_err("%s", jwm.get_paramnames()->get_name(paramnames::WETDRY));
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    return stockerrs::ERR_NO_ERROR;
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
    if (*in_gainmod >= 0)
        gainamount = gain - gain * gain_modsize
                     + (gain * gain_modsize * *in_gainmod);
    else
        gainamount = gain - gain * gain_modsize
                     + (gain * gain_modsize * *in_gainmod);
    output = filter[fpos];
    filter[fpos] = *in_signal * gainamount;
    fpos--;
    if (fpos < 0) fpos = filterarraymax - 1;
    out_output = output * wetdry + *in_signal * (1 - wetdry);
}

<<<<<<< HEAD


=======
>>>>>>> master
void delay::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::DELAY_TIME);
    relate_param(paramnames::GAIN);
    relate_param(paramnames::GAIN_MODSIZE);
    relate_param(paramnames::WETDRY);
}

