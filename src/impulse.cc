#include "../include/impulse.h"

impulse::impulse(const char* uname) :
 synthmod::base(synthmod::IMPULSE, uname, SM_HAS_OUT_OUTPUT),
 in_trig(0), out_output(0.0)
{
    register_output(output::OUT_OUTPUT);
}

void impulse::register_ui()
{
    register_input(input::IN_TRIG);
}

ui::moditem_list* impulse::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

impulse::~impulse()
{
}

void impulse::run()
{
    out_output = (*in_trig == OFF) ? 0.0 : 1.0;
}

const void* impulse::set_in(input::TYPE it, const void* o)
{
    return (it == input::IN_TRIG ? (in_trig = (STATUS*)o) : 0);
}

const void* impulse::get_in(input::TYPE it) const
{
    return (it == input::IN_TRIG ? in_trig : 0);
}

const void* impulse::get_out(output::TYPE ot) const
{
    return (ot == output::OUT_OUTPUT) ? &out_output : 0;
}

