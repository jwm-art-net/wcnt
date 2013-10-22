#include "../include/multiplier.h"
#include "../include/globals.h"

multiplier::multiplier(const char* uname) :
 synthmod::base(synthmod::MULTIPLIER, uname, SM_HAS_OUT_OUTPUT),
 in_signal1(0), in_signal2(0), out_output(0.00)
{
    register_output(output::OUT_OUTPUT);
}

void multiplier::register_ui()
{
    register_input(input::IN_SIGNAL1);
    register_input(input::IN_SIGNAL2);
}

ui::moditem_list* multiplier::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

multiplier::~multiplier()
{
}

const void* multiplier::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* multiplier::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL1:    return in_signal1 = (double*)o;
        case input::IN_SIGNAL2:    return in_signal2 = (double*)o;
        default: return 0;
    }
}

const void* multiplier::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL1:    return in_signal1;
        case input::IN_SIGNAL2:    return in_signal2;
        default: return 0;
    }
}

// go on... givvus a snare rush - rush - rush - rush.

