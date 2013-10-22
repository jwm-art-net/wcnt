#include "../include/difffilter.h"

diff_filter::diff_filter(const char* uname) :
 synthmod::base(synthmod::DIFFFILTER, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), out_output(0.0), oldinsig(0.0)
{
    register_output(output::OUT_OUTPUT);
}

void diff_filter::register_ui()
{
    register_input(input::IN_SIGNAL);
}

ui::moditem_list* diff_filter::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

diff_filter::~diff_filter()
{
}

void diff_filter::run()
{
    double insig = *in_signal;
    out_output = insig - oldinsig;
    oldinsig = insig;
}

const void* diff_filter::get_out(output::TYPE ot) const
{
    return (ot == output::OUT_OUTPUT ? &out_output : 0);
}

const void* diff_filter::set_in(input::TYPE it, const void* o)
{
    return (it == input::IN_SIGNAL ? (in_signal = (double*)o) : 0);
}

const void* diff_filter::get_in(input::TYPE it) const
{
    return (it == input::IN_SIGNAL ? in_signal : 0);
}

