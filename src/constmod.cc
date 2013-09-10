#ifndef CONSTMOD_H
#include "../include/constmod.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modparamlist.h"

constmod::constmod(char const* uname) :

 synthmod(
    synthmodnames::CONSTMOD,
    uname,
    SM_EMPTY_RUN | SM_HAS_OUT_OUTPUT),

 output(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    create_params();
}

constmod::~constmod()
{
}

void const* constmod::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot) 
    {
        case outputnames::OUT_OUTPUT: return &output;
        default: return 0;
    }
}

bool constmod::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt) {
        case paramnames::VALUE:
            output = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* constmod::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        // parameter /is/ output.
        case paramnames::VALUE: return &output;
        default: return 0;
    }
}

bool constmod::done_params = false;

void constmod::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(
     synthmodnames::CONSTMOD, paramnames::VALUE);
    done_params = true;
}

#endif
