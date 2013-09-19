#include "../include/trigdelay.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

trigdelay::trigdelay(char const* uname) :

 synthmod(
    synthmodnames::TRIGDELAY,
    uname,
    SM_HAS_OUT_TRIG),

 in_trig(0), out_trig(OFF), delay_time(0.0),
 past_trigs(0), pastmax(0), pastpos(0)
{
    register_input(inputnames::IN_TRIG);
    register_output(outputnames::OUT_TRIG);
    init_first();
}

trigdelay::~trigdelay()
{
    if (past_trigs)
        delete [] past_trigs;
}

void trigdelay::init()
{
    pastmax = (long)((delay_time * jwm.samplerate()) / 1000);
    past_trigs = new STATUS[pastmax];
    if(!past_trigs){
        invalidate();
        return;
    }
    for (long i = 0; i < pastmax; i++) past_trigs[i] = OFF;
    pastpos = pastmax - 1;
}

void const* trigdelay::get_out(outputnames::OUT_TYPE ot) const
{
    if (ot == outputnames::OUT_TRIG)
        return &out_trig;
    return 0;
}

void const* trigdelay::set_in(inputnames::IN_TYPE it, void const* o)
{
    if (it == inputnames::IN_TRIG) {
        in_trig = (STATUS*)o;
        return o;
    }
    return  0;
}

void const* trigdelay::get_in(inputnames::IN_TYPE it) const
{
    if (it == inputnames::IN_TRIG)
        return in_trig;
    return  0;
}

bool trigdelay::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    if (pt == paramnames::DELAY_TIME) {
        delay_time = *(double*)data;
        return true;
    }
    return false;
}

void const* trigdelay::get_param(paramnames::PAR_TYPE pt) const
{
    if (pt == paramnames::DELAY_TIME)
        return &delay_time;
    return 0;
}

stockerrs::ERR_TYPE trigdelay::validate()
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

void trigdelay::run()
{
    out_trig = past_trigs[pastpos];
    past_trigs[pastpos] = *in_trig;
    pastpos--;
    if (pastpos < 0) pastpos = pastmax - 1;
}

void trigdelay::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::DELAY_TIME);
}

