#ifndef RANDOMTRIGGER_H
#include "../include/randomtrigger.h"

randomtrigger::randomtrigger(char const* uname) :
 synthmod(synthmodnames::MOD_RANDTRIGGER, randomtrigger_count, uname),
 in_trig(0), out_trig(OFF), out_not_trig(OFF), probability(0.5),
 not_probability(0.5)
{
    get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_NOT_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_TRIG);
    randomtrigger_count++;
    create_params();
    srandom(time(0)); //srand(time(0));
}

randomtrigger::~randomtrigger()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* randomtrigger::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_TRIG:
        o = &out_trig;
        break;
    case outputnames::OUT_NOT_TRIG:
        o = &out_not_trig;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* randomtrigger::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_TRIG:
        i = in_trig = (STATUS*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool randomtrigger::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_PROBABILITY:
        probability = *(double*)data;
        retv = true;
        break;
    case paramnames::PAR_NOTPROBABILITY:
        not_probability = *(double*)data;
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* randomtrigger::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_PROBABILITY:
        return &probability;
    case paramnames::PAR_NOTPROBABILITY:
        return &not_probability;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE randomtrigger::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_PROBABILITY,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_PROBABILITY);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!pl->validate(this, paramnames::PAR_NOTPROBABILITY,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_NOTPROBABILITY);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    return stockerrs::ERR_NO_ERROR;
}

void randomtrigger::run()
{
    if (*in_trig == ON) {
        if (rand() >= RAND_MAX * (1.00 - probability))
            out_trig = ON;
        else if (rand() >= RAND_MAX * (1 - not_probability))
            out_not_trig = ON;
    } else {
        if (out_trig == ON)
            out_trig = OFF;
        if (out_not_trig == ON)
            out_not_trig = OFF;
    }
}

int randomtrigger::randomtrigger_count = 0;

bool randomtrigger::done_params = false;

void randomtrigger::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_RANDTRIGGER, paramnames::PAR_PROBABILITY);
    get_paramlist()->add_param(
     synthmodnames::MOD_RANDTRIGGER, paramnames::PAR_NOTPROBABILITY);
    done_params = true;
}

#endif
