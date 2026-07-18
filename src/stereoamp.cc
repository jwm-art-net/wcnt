#include "../include/stereoamp.h"

stereo_amp::stereo_amp(const char* uname) :
 synthmod::base(synthmod::STEREOAMP, uname, SM_HAS_STEREO_OUTPUT),
 in_left(0), in_right(0), in_amp_eg(0), in_amp_mod(0),
 out_left(0), out_right(0),
 gain(1.0), balance(0.0), amp_modsize(0.0), clip_level(1.0)
{
    register_output(output::OUT_LEFT);
    register_output(output::OUT_RIGHT);
}

void stereo_amp::register_ui()
{
    register_input(input::IN_SIGNAL)  ->set_flags(ui::UI_OPTION1);
    register_input(input::IN_LEFT)    ->set_flags(ui::UI_OPT2_DUP);
    register_input(input::IN_RIGHT)   ->set_flags(ui::UI_OPT2_DUP);
    register_input(input::IN_EG)      ->set_flags(ui::UI_OPTIONAL);
    register_param(param::GAIN);
    register_param(param::BALANCE)    ->set_flags(ui::UI_OPTIONAL);
    register_input(input::IN_AMP_MOD) ->set_flags(ui::UI_GROUP1);
    register_param(param::AMP_MODSIZE)->set_flags(ui::UI_GROUP1);
    register_param(param::CLIP_LEVEL) ->set_flags(ui::UI_OPTIONAL);
}

ui::moditem_list* stereo_amp::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

stereo_amp::~stereo_amp()
{
}

const void* stereo_amp::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_LEFT:     return &out_left;
        case output::OUT_RIGHT:    return &out_right;
       default: return 0;
    }
}

const void* stereo_amp::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_left = in_right = (double*)o;
        case input::IN_LEFT:   return in_left = (double*)o;
        case input::IN_RIGHT:  return in_right = (double*)o;
        case input::IN_EG:     return in_amp_eg = (double*)o;
        case input::IN_AMP_MOD:return in_amp_mod = (double*)o;
        default: return 0;
    }
}

const void* stereo_amp::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_LEFT:    return in_left;
        case input::IN_RIGHT:   return in_right;
        case input::IN_EG:      return in_amp_eg;
        case input::IN_AMP_MOD: return in_amp_mod;
        default: return 0;
    }
}

bool stereo_amp::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::GAIN:
            gain = *(double*)data;
            return true;
        case param::BALANCE:
            balance = *(double*)data;
            return true;
        case param::AMP_MODSIZE:
            amp_modsize = *(double*)data;
            return true;
        case param::CLIP_LEVEL:
            clip_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* stereo_amp::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::GAIN:        return &gain;
        case param::BALANCE:     return &balance;
        case param::AMP_MODSIZE: return &amp_modsize;
        case param::CLIP_LEVEL:  return &clip_level;
        default: return 0;
    }
}

errors::TYPE stereo_amp::validate()
{
    if (!validate_param(param::AMP_MODSIZE, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    if (!validate_param(param::BALANCE, errors::RANGE_M1_1))
        return errors::RANGE_M1_1;

    if (!validate_param(param::CLIP_LEVEL, errors::NEG_OR_ZERO))
        return errors::NEG_OR_ZERO;

    return errors::NO_ERROR;
}

void stereo_amp::run()
{
    double gg = (gain * (1 - amp_modsize) + gain * *in_amp_mod * amp_modsize) * *in_amp_eg;
    double ll, rl;
    if (balance < 0) {
        ll = 1;
        rl = 1 + balance;
    }
    else {
        ll = 1 - balance;
        rl = 1;
    }

    out_left = *in_left * gg * ll;
    out_right = *in_right * gg * rl;

    if (out_left < -clip_level)
        out_left = -clip_level;
    else if (out_left > clip_level)
        out_left = clip_level;
    if (out_right < -clip_level)
        out_right = -clip_level;
    else if (out_right > clip_level)
        out_right = clip_level;
}

