#include "../include/wcntsignal.h"

wcnt_signal::wcnt_signal(const char* uname) :
synthmod::base(synthmod::WCNTSIGNAL, uname, SM_HAS_OUT_OUTPUT |
                                            SM_EMPTY_RUN |
                                            SM_HAS_OUT_TRIG |
                                            SM_PASSTHROUGH),
 in_signal(0)
{
    register_output(output::OUT_OUTPUT);
}

void wcnt_signal::register_ui()
{
    register_input(input::IN_SIGNAL);
}

ui::moditem_list* wcnt_signal::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

wcnt_signal::~wcnt_signal()
{
}

const void* wcnt_signal::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return in_signal;
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



