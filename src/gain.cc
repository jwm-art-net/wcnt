#include "../include/gain.h"


gain::gain(synthmod::base* _sm) :
 modpart::base(_sm),
 out(0.0f),
 in_signal(0), in_mod(0),
 level(0.0), mod_amount(0.0),
 center(0.0), half_range(0.0)
{
}

void gain::register_ui()
{
    register_sm_input(input::IN_SIGNAL);
    register_sm_param(param::GAIN);
    register_sm_input(input::IN_GAIN_MOD)->set_flags(ui::UI_GROUP1);
    register_sm_param(param::GAIN_MODSIZE)->set_flags(ui::UI_GROUP1);
}

gain::~gain()
{
}

const void* gain::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL:  return in_signal = (double*)o;
        case input::IN_GAIN_MOD:return in_mod = (double*)o;
        default: return 0;
    }
}

const void* gain::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL:  return in_signal;
        case input::IN_GAIN_MOD:return in_mod;
        default: return 0;
    }
}

bool gain::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::GAIN:
            level = *(double*)data;
            return true;
        case param::GAIN_MODSIZE:
            mod_amount = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* gain::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::GAIN:         return &level;
        case param::GAIN_MODSIZE:   return &mod_amount;
        default: return 0;
    }
}

void gain::init()
{
    double range = level * mod_amount;
    double low_level = level - range;
    half_range = range / 2.0f;
    center = low_level + half_range;
}


errors::TYPE gain::validate()
{
    if (!validate_sm_param(param::GAIN_MODSIZE, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    return errors::NO_ERROR;
}
