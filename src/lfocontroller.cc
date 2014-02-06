#include "../include/lfocontroller.h"
#include "../include/conversions.h"

lfo_controller::lfo_controller(const char* uname) :
 synthmod::base(synthmod::LFOCONTROL, uname, SM_HAS_OUT_OUTPUT),
 in_trig(0), in_wave(0), in_amp_mod(0), out_preampmod(0), output(0),
 delay_time(0), ramp_time(0), start_level(0), end_level(1.0),
 response_time(0), amp_modsize(0),
 total_resp_samples(0), resp_samples(0),
 resp_step(0.0f), resp_level(0.0f), resp_level_grab(0.0f),
 total_delay_samples(0), delay_samples(0),
 total_ramp_samples(0), ramp_samples(0), ramp_step(0.0f), ramp_level(0.0f),
 amp_center(0.0f), amp_half_range(0.0f)

{
    register_output(output::OUT_OUTPUT);
    register_output(output::OUT_PRE_AMP_MOD);
}

void lfo_controller::register_ui()
{
    register_input(input::IN_TRIG);
    register_param(param::START_LEVEL);
    register_param(param::DELAY_TIME);
    register_param(param::RAMP_TIME);
    register_param(param::END_LEVEL);
    register_input(input::IN_WAVE);
    register_input(input::IN_AMP_MOD)->set_flags(ui::UI_GROUP1);
    register_param(param::AMP_MODSIZE)->set_flags(ui::UI_GROUP1);
    register_param(param::RESPONSE_TIME);
}

ui::moditem_list* lfo_controller::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

lfo_controller::~lfo_controller()
{
}

const void* lfo_controller::get_out(int ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT:       return &output;
        case output::OUT_PRE_AMP_MOD:  return &out_preampmod;
        default: return 0;
    }
}

const void* lfo_controller::set_in(int it, const void* o)
{
    switch(it)
    {
        case input::IN_TRIG:       return in_trig = (STATUS*)o;
        case input::IN_WAVE:       return in_wave = (double*)o;
        case input::IN_AMP_MOD:    return in_amp_mod = (double*)o;
        default: return 0;
    }
}

const void* lfo_controller::get_in(int it) const
{
    switch(it)
    {
        case input::IN_TRIG:   return in_trig;
        case input::IN_WAVE:   return in_wave;
        case input::IN_AMP_MOD:return in_amp_mod;
        default: return 0;
    }
}

bool lfo_controller::set_param(int pt, const void* data)
{
    switch(pt)
    {
        case param::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        case param::RAMP_TIME:
            ramp_time = *(double*)data;
            return true;
        case param::START_LEVEL:
            start_level = *(double*)data;
            return true;
        case param::END_LEVEL:
            end_level = *(double*)data;
            return true;
        case param::RESPONSE_TIME:
            response_time = *(double*)data;
            return true;
        case param::AMP_MODSIZE:
            amp_modsize = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* lfo_controller::get_param(int pt) const
{
    switch(pt)
    {
        case param::DELAY_TIME:    return &delay_time;
        case param::RAMP_TIME:     return &ramp_time;
        case param::START_LEVEL:   return &start_level;
        case param::END_LEVEL:     return &end_level;
        case param::RESPONSE_TIME: return &response_time;
        case param::AMP_MODSIZE:   return &amp_modsize;
        default: return 0;
    }
}

errors::TYPE lfo_controller::validate()
{
    if (!validate_param(param::DELAY_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::RAMP_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (!validate_param(param::RESPONSE_TIME, errors::NEGATIVE))
        return errors::NEGATIVE;

    if (response_time < 0) {
        sm_err("%s less than zero.", param::names::get(
                                                param::RESPONSE_TIME));
        invalidate();
    }
    return errors::NO_ERROR;
}

void lfo_controller::init()
{
    total_resp_samples = ms_to_samples(response_time);
    if (total_resp_samples > 0)
        resp_step = 1.0f / total_resp_samples;

    total_delay_samples = ms_to_samples(delay_time);
    total_ramp_samples = ms_to_samples(ramp_time);
    if (total_ramp_samples > 0)
        ramp_step = (end_level - start_level) / total_ramp_samples;
    else
        ramp_level = end_level;
    amp_half_range = amp_modsize / 2.0f;
    amp_center = 1.0 - amp_half_range;
}

void lfo_controller::run()
{
    if (*in_trig == ON) {
        if (total_resp_samples > 0) {
            resp_samples = total_resp_samples;
            resp_level_grab = out_preampmod;
            resp_level = 1.0f;
        }
        delay_samples = total_delay_samples;
        if (total_ramp_samples > 0) {
            ramp_samples = total_ramp_samples;
            ramp_level = start_level;
        }
    }
    else if (delay_samples > 0)
        delay_samples--;
    else if (ramp_samples > 0) {
        ramp_level += ramp_step;
        if (--ramp_samples == 0)
            ramp_level = 1.0f;
    }

    if (resp_samples > 0) {
        out_preampmod = resp_level_grab * resp_level
                        + *in_wave * ramp_level * (1.0f - resp_level);
        resp_level -= resp_step;
        --resp_samples;
    }
    else
        out_preampmod = *in_wave * ramp_level;

    if (out_preampmod > 1.0f)
        out_preampmod = 1.0f;
    else if (out_preampmod < -1.0f)
        out_preampmod = -1.0f;

    output = out_preampmod * (amp_center + amp_half_range * *in_amp_mod);
}
