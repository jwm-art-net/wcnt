#include "../include/contraster.h"

contraster::contraster(const char* uname) :
 synthmod::base(synthmod::CONTRASTER, uname, SM_HAS_OUT_OUTPUT),
 out_output(0.0), in_signal(0), in_power_mod(0),
 in_rude_switch_trig(0), power_min(1.0), power_max(2.0), rude_mode(OFF),
 wetdry(0), power(0.0), power_mod(0), powerrad(0.0), output(0)
{
    register_output(output::OUT_OUTPUT);
}

void contraster::register_ui()
{
    register_input(input::IN_SIGNAL);

    register_param(param::POWER)->          set_flags(ui::UI_OPTION1);
    register_input(input::IN_POWER_MOD)->   set_flags(ui::UI_OPTION1 | ui::UI_OPT_DUMMY);

    register_input(input::IN_POWER_MOD)->   set_flags(ui::UI_OPTION2 | ui::UI_OPT_DUPLICATE);
    register_param(param::POWER_MIN)->      set_flags(ui::UI_OPTION2 | ui::UI_OPT_DUPLICATE);
    register_param(param::POWER_MAX)->      set_flags(ui::UI_OPTION2 | ui::UI_OPT_DUPLICATE);

    register_param(param::RUDE_MODE)->      set_flags(ui::UI_GROUP1);
    register_input(input::IN_RUDE_SWITCH_TRIG)->
                                            set_flags(ui::UI_GROUP1 | ui::UI_OPTIONAL);
    register_param(param::WETDRY);
}

ui::moditem_list* contraster::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

contraster::~contraster()
{
}

const void* contraster::get_out(int ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* contraster::set_in(int it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL:
            return in_signal = (double*)o;
        case input::IN_POWER_MOD:
            return in_power_mod = (double*)o;
        case input::IN_RUDE_SWITCH_TRIG:
            return in_rude_switch_trig = (STATUS*)o;
        default:
            return 0;
    }
}

const void* contraster::get_in(int it) const
{
    switch(it)
    {
        case input::IN_SIGNAL:
            return in_signal;
        case input::IN_POWER_MOD:
            return in_power_mod;
        case input::IN_RUDE_SWITCH_TRIG:
            return in_rude_switch_trig;
        default:
            return 0;
    }
}

bool contraster::set_param(int pt, const void* data)
{
    switch(pt)
    {
        case param::POWER:
            power_min = power_max = *(double*)data;
            return true;
        case param::POWER_MIN:
            power_min = *(double*)data;
            return true;
        case param::POWER_MAX:
            power_max = *(double*)data;
            return true;
        case param::RUDE_MODE:
            rude_mode = *(STATUS*)data;
            return true;
        case param::WETDRY:
            wetdry = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* contraster::get_param(int pt) const
{
    switch(pt)
    {
    case param::POWER_MIN:
        return &power_min;
    case param::POWER_MAX:
        return &power_max;
    case param::RUDE_MODE:
        return &rude_mode;
    case param::WETDRY:
        return &wetdry;
    default:
        return 0;
    }
}

errors::TYPE contraster::validate()
{
    if (!validate_param(param::WETDRY, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    return errors::NO_ERROR;
}

void contraster::init()
{
    powerrad = (power_max - power_min) / 2;
}

void contraster::run()
{
    power_mod = (*in_power_mod < 0) ? -*in_power_mod : *in_power_mod;
    power = power_min + powerrad * power_mod;
    output = *in_signal * power;
    if (*in_rude_switch_trig) {
        if (rude_mode)
            rude_mode = OFF;
        else
            rude_mode = ON;
    }
    if (rude_mode) {
        while (output > 1.0 || output < -1.0) {
            if (output > 1.0)
                output = output - 1.0;
            if (output < -1.0)
                output = output + 1.0;
        }
    }
    else {
        while (output > 1.0 || output < -1.0) {
            if (output > 1.0)
                output = 1.0 - (output - 1);
            if (output < -1.0)
                output = -1.0 - (output + 1);
        }
    }
    out_output = output * wetdry + *in_signal * (1.0 - wetdry);
}

