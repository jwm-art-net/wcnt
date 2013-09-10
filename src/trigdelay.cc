#ifndef TRIGDELAY_H
#include "../include/trigdelay.h"

trigdelay::trigdelay(char const* uname) :
 synthmod(synthmodnames::MOD_TRIGDELAY, trigdelay_count, uname),
 in_trig(0), out_trig(OFF), delay_time(0.0),
 past_trigs(0), pastmax(0), pastpos(0)
{
    get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_TRIG);
    trigdelay_count++;
    create_params();
}

trigdelay::~trigdelay()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
    if (past_trigs)
        delete [] past_trigs;
}

void trigdelay::init()
{
    pastmax = (long)((delay_time * audio_samplerate) / 1000);
    past_trigs = new STATUS[pastmax];
    for (long i = 0; i < pastmax; i++) past_trigs[i] = OFF;
    pastpos = pastmax - 1;
}

void const* trigdelay::get_out(outputnames::OUT_TYPE ot)
{
    if (ot == outputnames::OUT_TRIG)
        return &out_trig;
    return 0;
}

void const* trigdelay::set_in(inputnames::IN_TYPE it, void const* o)
{
    if (it == inputnames::IN_TRIG) {
        set_input_trig((STATUS*)o);
        return o;
    }
    return  0;
}

void const* trigdelay::get_in(inputnames::IN_TYPE it)
{
    if (it == inputnames::IN_TRIG)
        return get_input_trig();
    return  0;
}

bool trigdelay::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    if (pt == paramnames::PAR_DELAY_TIME) {
        set_delay_time(*(double*)data);
        return true;
    }
    return false;
}

void const* trigdelay::get_param(paramnames::PAR_TYPE pt)
{
    if (pt == paramnames::PAR_DELAY_TIME) {
        return &delay_time;
    }
    return 0;
}

stockerrs::ERR_TYPE trigdelay::validate()
{
    if (!get_paramlist()->validate(this, paramnames::PAR_DELAY_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_DELAY_TIME);
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

int trigdelay::trigdelay_count = 0;

bool trigdelay::done_params = false;

void trigdelay::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_TRIGDELAY, paramnames::PAR_DELAY_TIME);
    done_params = true;
}

#endif
