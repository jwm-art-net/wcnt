#include "../include/trigdelay.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

trigdelay::trigdelay(const char* uname) :
 synthmod(module::TRIGDELAY, uname, SM_HAS_OUT_TRIG),
 in_trig(0), out_trig(OFF), delay_time(0.0),
 past_trigs(0), pastmax(0), pastpos(0)
{
    register_input(input::IN_TRIG);
    register_output(output::OUT_TRIG);
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

const void* trigdelay::get_out(output::TYPE ot) const
{
    return (ot == output::OUT_TRIG ? &out_trig : 0);
}

const void* trigdelay::set_in(input::TYPE it, const void* o)
{
    return (it == input::IN_TRIG ? (in_trig = (STATUS*)o) : 0);
}

const void* trigdelay::get_in(input::TYPE it) const
{
    return (it == input::IN_TRIG ? in_trig : 0);
}

bool trigdelay::set_param(param::TYPE pt, const void* data)
{
    if (pt == param::DELAY_TIME) {
        delay_time = *(double*)data;
        return true;
    }
    return false;
}

const void* trigdelay::get_param(param::TYPE pt) const
{
    return (pt == param::DELAY_TIME ? &delay_time : 0);
}

stockerrs::ERR_TYPE trigdelay::validate()
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
    register_param(param::DELAY_TIME);
}

