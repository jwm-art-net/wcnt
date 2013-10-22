#include "../include/delay.h"
#include "../include/globals.h"

delay::delay(const char* uname) :
 synthmod::base(synthmod::DELAY, uname, SM_HAS_OUT_OUTPUT),
 gain(this),
 out_output(0), delay_time(0),
 wetdry(0), output(0), filter(0), filterarraymax(0),
 fpos(0), filtertotal(0)
{
    register_output(output::OUT_OUTPUT);
}

void delay::register_ui()
{
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
    default:
        return  gain::set_in(it, o);
    }
}

const void* delay::get_in(input::TYPE it) const
{
    switch(it)
    {
    default:
        return gain::get_in(it);
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
        return gain::set_param(pt, data);
    }
}

const void* delay::get_param(param::TYPE pt) const
{
    switch(pt)
    {
    case param::DELAY_TIME:    return &delay_time;
    case param::WETDRY:        return &wetdry;
    default:
        return gain::get_param(pt);
    }
}

errors::TYPE delay::validate()
{
    if (!validate_param(param::DELAY_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::WETDRY, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    return gain::validate();
}

void delay::init()
{
    filterarraymax = (long)((delay_time * wcnt::jwm.samplerate()) / 1000);
    filter = new double[filterarraymax];
    if (!filter){
        invalidate();
        return;
    }
    for (long i = 0; i < filterarraymax; i++)
        filter[i] = 0;
    fpos = filterarraymax - 1;
    gain::init();
}

void delay::run()
{
    gain::run();
    output = filter[fpos];
    out_output = output * wetdry + gain::out * (1 - wetdry);
    filter[fpos] = gain::out;
    if (--fpos < 0)
        fpos = filterarraymax - 1;
}

