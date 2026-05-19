#include "../include/monoamp.h"
#include "../include/globals.h"

mono_amp::mono_amp(const char* uname) :
 synthmod::base(synthmod::MONOAMP, uname, SM_HAS_OUT_OUTPUT),
 gain(this),
 in_signal(0), in_amp_eg(0),
 out_output(0),
 clip_level(1.0), eg_amount(1.0), eg_base(0.0)
{
    register_output(output::OUT_OUTPUT);
}

void mono_amp::register_ui()
{
    gain::register_ui();
    register_input(input::IN_EG)->set_flags(ui::UI_OPTIONAL);;
    register_param(param::EG_AMOUNT)->set_flags(ui::UI_OPTIONAL);;
    register_param(param::CLIP_LEVEL)->set_flags(ui::UI_OPTIONAL);;
}

ui::moditem_list* mono_amp::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

mono_amp::~mono_amp()
{
}

const void* mono_amp::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* mono_amp::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_EG: return in_amp_eg = (double*)o;
        default:
            return gain::set_in(it, o);
    }
}

const void* mono_amp::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_EG: return in_amp_eg;
        default:
            return gain::get_in(it);
    }
}

bool mono_amp::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::CLIP_LEVEL:
            clip_level = *(double*)data;
            return true;
        case param::EG_AMOUNT:
            eg_amount = *(double*)data;
            return true;
        default:
            return gain::set_param(pt, data);
    }
}

const void* mono_amp::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::CLIP_LEVEL: return &clip_level;
        case param::EG_AMOUNT:  return &eg_amount;
        default:
            return gain::get_param(pt);
    }
}

errors::TYPE mono_amp::validate()
{
    if (!validate_param(param::CLIP_LEVEL, errors::NEG_OR_ZERO))
        return errors::NEG_OR_ZERO;
    if (!validate_param(param::EG_AMOUNT, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    eg_base = 1 - eg_amount;

    return gain::validate();
}

void mono_amp::run()
{
    gain::run();
    out_output = gain::out * (eg_base + eg_amount * *in_amp_eg);
    if (out_output < -clip_level)
        out_output = -clip_level;
    else if (out_output > clip_level)
        out_output = clip_level;
}


