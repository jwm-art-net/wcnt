#include "../include/pan.h"
#include "../include/globals.h"

pan::pan(const char* uname) :
 synthmod::base(synthmod::PAN, uname, SM_HAS_STEREO_OUTPUT),
 in_signal(0), in_pan_mod(0), out_l(0), out_r(0), panpos(0),
 pan_modsize(0), pan_mod(0), pan_pos(0)
{
    register_output(output::OUT_LEFT);
    register_output(output::OUT_RIGHT);
}

void pan::register_ui()
{
    register_input(input::IN_SIGNAL);
    register_param(param::PAN);
    register_input(input::IN_PAN_MOD)->set_flags(ui::UI_GROUP1);
    register_param(param::PAN_MODSIZE)->set_flags(ui::UI_GROUP1);
}

ui::moditem_list* pan::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

pan::~pan()
{
}

const void* pan::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_LEFT: return &out_l;
        case output::OUT_RIGHT:return &out_r;
        default: return 0;
    }
}

const void* pan::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal = (double*)o;
        case input::IN_PAN_MOD:return in_pan_mod = (double*)o;
        default: return 0;
    }
}

const void* pan::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
        case input::IN_PAN_MOD:return in_pan_mod;
        default: return 0;
    }
}

bool pan::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::PAN:
            panpos = *(double*)data;
            return true;
        case param::PAN_MODSIZE:
            pan_modsize = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* pan::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::PAN:        return &panpos;
        case param::PAN_MODSIZE:return &pan_modsize;
        default: return 0;
    }
}

errors::TYPE pan::validate()
{
    if (!validate_param(param::PAN, errors::RANGE_M1_1))
        return errors::RANGE_M1_1;

    if (!validate_param(param::PAN_MODSIZE, errors::RANGE_0_1))
        return errors::RANGE_0_1;

    return errors::NO_ERROR;
}

void pan::run()
{
    if ((pan_mod = *in_pan_mod) > 1) pan_mod = 1;
    else if (pan_mod < -1) pan_mod = -1;
//    pan_pos = panpos * (1 - pan_modsize) + pan_mod * pan_modsize;
    pan_pos = panpos + pan_modsize * pan_mod;
    if (pan_pos > 0) {
        out_l = *in_signal;
        out_r = *in_signal * (1 - pan_pos);
    } else {
        out_l = *in_signal * (1 + pan_pos);
        out_r = *in_signal;
    }
}

