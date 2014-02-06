#include "../include/invert.h"

invert::invert(const char* uname) :
 synthmod::base(synthmod::INVERT, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), out_output(0.00)
{
    register_output(output::OUT_OUTPUT);
}

void invert::register_ui()
{
    register_input(input::IN_SIGNAL);
}

ui::moditem_list* invert::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

invert::~invert()
{
}

const void* invert::get_out(int ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* invert::set_in(int it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

const void* invert::get_in(int it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

// I'm not going to be dancing here for too much longer.
