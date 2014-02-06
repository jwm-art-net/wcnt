#include "../include/simpledelay.h"
#include "../include/globals.h"

simple_delay::simple_delay(const char* uname) :
 synthmod::base(synthmod::SIMPLEDELAY, uname, SM_HAS_OUT_OUTPUT),
 in_signal(0), out_output(0), delay_time(0), output(0),
 filter(0), filterarraymax(0), fpos(0), filtertotal(0)
{
    register_output(output::OUT_OUTPUT);
}

void simple_delay::register_ui()
{
    register_input(input::IN_SIGNAL);
    register_param(param::DELAY_TIME);
}

ui::moditem_list* simple_delay::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

simple_delay::~simple_delay()
{
    if (filter)
        delete [] filter;
}

void simple_delay::init()
{
    filterarraymax = (long)((delay_time * wcnt::jwm.samplerate()) / 1000);
    filter = new double[filterarraymax];
    if (!filter){
        invalidate();
        return;
    }
    for (long i = 0; i < filterarraymax; i++) filter[i] = 0;
    fpos = filterarraymax - 1;
}

const void* simple_delay::get_out(int ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* simple_delay::set_in(int it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal = (double*)o;
        default: return  0;
    }
}

const void* simple_delay::get_in(int it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool simple_delay::set_param(int pt, const void* data)
{
    switch(pt)
    {
        case param::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* simple_delay::get_param(int pt) const
{
    switch(pt)
    {
        case param::DELAY_TIME: return &delay_time;
        default: return 0;
    }
}

errors::TYPE simple_delay::validate()
{
    if (!validate_param(param::DELAY_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    return errors::NO_ERROR;
}

void simple_delay::run()
{
    out_output = filter[fpos];
    filter[fpos] = *in_signal;
    fpos--;
    if (fpos < 0) fpos = filterarraymax - 1;
}

