#include "../include/delay.h"
#include "../include/globals.h"

delay::delay(const char* uname) :
 synthmod::base(synthmod::DELAY, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), out_output(0), delay_time(0),
 wetdry(0), filter(0), filterarraymax(0),
 fpos(0), filtertotal(0)
{
    register_output(output::OUT_OUTPUT);
}

void delay::register_ui()
{
    register_input(input::IN_SIGNAL);
    register_param(param::DELAY_TIME);
    register_param(param::WETDRY);
}

ui::moditem_list* delay::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

delay::~delay()
{
    if (filter)
        delete [] filter;
}

const void* delay::get_out(output::TYPE ot) const
{
    switch(ot)
    {
    case output::OUT_OUTPUT: return &out_output;
    default: return 0;
    }
}

const void* delay::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL:  return in_signal = (double*)o;
        default:
            return 0;
    }
}

const void* delay::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL:  return in_signal;
        default:
            return 0;
    }
}

bool delay::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::DELAY_TIME:
        delay_time = *(double*)data;
        return true;
    case param::WETDRY:
        wetdry = *(double*)data;
        return true;
    default:
        return false;
    }
}

const void* delay::get_param(param::TYPE pt) const
{
    switch(pt)
    {
    case param::DELAY_TIME:    return &delay_time;
    case param::WETDRY:        return &wetdry;
    default:
        return 0;
    }
}

errors::TYPE delay::validate()
{
    if (!validate_param(param::DELAY_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::WETDRY, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    return errors::NO_ERROR;
}

void delay::init()
{
    filterarraymax = (long)((delay_time * wcnt::jwm.samplerate()) / 1000.0);
    filter = new double[filterarraymax];
    if (!filter){
        invalidate();
        return;
    }
    for (long i = 0; i < filterarraymax; i++)
        filter[i] = 0;
    fpos = filterarraymax - 1;
}

void delay::run()
{
    out_output = filter[fpos] * wetdry + *in_signal * (1 - wetdry);
    filter[fpos] = *in_signal;
    if (--fpos < 0)
        fpos = filterarraymax - 1;
}

