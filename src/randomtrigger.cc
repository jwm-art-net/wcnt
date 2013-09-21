#include "../include/randomtrigger.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

#include <cstdlib>

randomtrigger::randomtrigger(char const* uname) :

 synthmod(
    synthmodnames::RANDTRIGGER,
     uname,
    SM_HAS_OUT_TRIG),

 in_trig(0), out_trig(OFF), out_not_trig(OFF), probability(0.5),
 not_probability(0.5)
{
    register_input(inputnames::IN_TRIG);
    register_output(outputnames::OUT_TRIG);
    register_output(outputnames::OUT_NOT_TRIG);
    init_first();
    srandom(time(0)); //srand(time(0));
}

randomtrigger::~randomtrigger()
{
}

void const* randomtrigger::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_TRIG:     return &out_trig;
        case outputnames::OUT_NOT_TRIG: return &out_not_trig;
        default: return 0;
    }
}

void const* randomtrigger::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig = (STATUS*)o;
        default: return 0;
    }
}

void const* randomtrigger::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG: return in_trig;
        default: return 0;
    }
}

bool randomtrigger::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::PROBABILITY:
            probability = *(double*)data;
            return true;
        case paramnames::NOTPROBABILITY:
            not_probability = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* randomtrigger::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::PROBABILITY:       return &probability;
        case paramnames::NOTPROBABILITY:    return &not_probability;
        default: return 0;
    }
}

stockerrs::ERR_TYPE randomtrigger::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::PROBABILITY,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg =
         paramnames::get_name(paramnames::PROBABILITY);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::NOTPROBABILITY,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg =
         paramnames::get_name(paramnames::NOTPROBABILITY);
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

void randomtrigger::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::PROBABILITY);
    register_param(paramnames::NOTPROBABILITY);
}

