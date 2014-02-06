#include "../include/wcntsignal.h"

wcnt_signal::wcnt_signal(const char* uname) :
 synthmod::base(synthmod::WCNTSIGNAL, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), out_output(0.0), level(1.0)
{
    register_output(output::OUT_OUTPUT);
    register_output(output::OUT_THROUGH);
}

void wcnt_signal::register_ui()
{
    register_input(input::IN_SIGNAL);
    register_param(param::LEVEL)->set_flags(ui::UI_OPTIONAL);
}

ui::moditem_list* wcnt_signal::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

wcnt_signal::~wcnt_signal()
{
}

const void* wcnt_signal::get_out(int ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        case output::OUT_THROUGH:return in_signal;
        default: return 0;
    }
}

const void* wcnt_signal::set_in(int it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

const void* wcnt_signal::get_in(int it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool wcnt_signal::set_param(int pt, const void* data)
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

const void* wcnt_signal::get_param(int pt) const
{
    switch(pt)
    {
        case param::LEVEL: return &level;
        default: return 0;
    }
}

