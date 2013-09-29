#include "../include/wcntsignal.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/connectorlist.h"

wcnt_signal::wcnt_signal(const char* uname) :
 synthmod(module::WCNTSIGNAL, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), out_output(0.0), level(0.0)
{
    register_input(input::IN_SIGNAL);
    register_output(output::OUT_OUTPUT);
    register_output(output::OUT_THROUGH);
    init_first();
}

wcnt_signal::~wcnt_signal()
{
}

const void* wcnt_signal::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        case output::OUT_THROUGH:return in_signal;
        default: return 0;
    }
}

const void* wcnt_signal::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

const void* wcnt_signal::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool wcnt_signal::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::LEVEL:
            level = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* wcnt_signal::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::LEVEL: return &level;
        default: return 0;
    }
}

void wcnt_signal::init_first()
{
    if (done_first())
        return;
    register_param(param::LEVEL);
}

