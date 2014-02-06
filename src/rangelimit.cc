#include "../include/rangelimit.h"

range_limit::range_limit(const char* uname) :
 synthmod::base(synthmod::RANGELIMIT, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), out_output(0), sigrangehi(0), sigrangelo(0)
{
    register_output(output::OUT_OUTPUT);
}

void range_limit::register_ui()
{
    register_input(input::IN_SIGNAL);
    register_param(param::SIG_RANGE_HI);
    register_param(param::SIG_RANGE_LO);
}

ui::moditem_list* range_limit::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

range_limit::~range_limit()
{
}

const void* range_limit::get_out(int ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* range_limit::set_in(int it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL:
            return in_signal = (double*)o;
        default:
            return 0;
    }
}

const void* range_limit::get_in(int it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool range_limit::set_param(int pt, const void* data)
{
    switch(pt)
    {
        case param::SIG_RANGE_HI:
            sigrangehi = *(double*)data;
            return true;
        case param::SIG_RANGE_LO:
            sigrangelo = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* range_limit::get_param(int pt) const
{
    switch(pt)
    {
        case param::SIG_RANGE_HI: return &sigrangehi;
        case param::SIG_RANGE_LO: return &sigrangelo;
        default: return 0;
    }
}

void range_limit::init()
{
    if (sigrangelo > sigrangehi) {
        double tmp = sigrangehi;
        sigrangehi = sigrangelo;
        sigrangelo = tmp;
    }
}

void range_limit::run()
{
    if ((out_output = *in_signal) < sigrangelo)
        out_output = sigrangelo;
    else if (out_output > sigrangehi)
        out_output = sigrangehi;
}

