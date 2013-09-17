#include "../include/wcntsignal.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/connectorlist.h"

wcnt_signal::wcnt_signal(char const* uname) :
 synthmod(synthmodnames::WCNTSIGNAL, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), out_output(0.0), level(0.0)
{
    add_output(outputnames::OUT_OUTPUT);
    add_output(outputnames::OUT_THROUGH);
    add_input(inputnames::IN_SIGNAL);
    create_params();
}

wcnt_signal::~wcnt_signal()
{
}

void const* wcnt_signal::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        case outputnames::OUT_THROUGH:return in_signal;
        default: return 0;
    }
}

void const* wcnt_signal::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

void const* wcnt_signal::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool wcnt_signal::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::LEVEL:
            level = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* wcnt_signal::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::LEVEL: return &level;
        default: return 0;
    }
}

void wcnt_signal::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::LEVEL);
}

