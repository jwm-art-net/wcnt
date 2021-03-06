#include "../include/fast_lookahead_limiter.h"
#ifdef WITH_LADSPA
#include "../include/globals.h"

fast_lookahead_limiter::fast_lookahead_limiter(const char* uname) :

 synthmod::base(synthmod::FAST_LOOKAHEAD_LIMITER, uname,
                                                    SM_HAS_STEREO_OUTPUT),
 in_left(0), in_right(0),
 out_left(0), out_right(0),
 gain_db(0), limit_db(0), release_secs(0.5),
 l_descriptor(0), l_inst_handle(0),
 l_gain_db(0), l_limit_db(0), l_release_secs(0),
 l_out_atten(0),
 l_in_left(0), l_in_right(0), l_out_left(0), l_out_right(0),
 l_out_latency(0)
{
    register_output(output::OUT_LEFT);
    register_output(output::OUT_RIGHT);
}

void fast_lookahead_limiter::register_ui()
{
    register_input(input::IN_LEFT);
    register_input(input::IN_RIGHT)->set_connect_as(input::IN_LEFT)
                                   ->set_flags(ui::UI_OPTIONAL);
    register_param(param::GAIN_DB);
    register_param(param::LIMIT_DB);
    register_param(param::RELEASE_SECS);
}

ui::moditem_list* fast_lookahead_limiter::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

fast_lookahead_limiter::~fast_lookahead_limiter()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_in_left)  delete [] l_in_left;
    if (l_in_right) delete [] l_in_right;
    if (l_out_left) delete [] l_out_left;
    if (l_out_right)delete [] l_out_right;
}

const void*
fast_lookahead_limiter::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_LEFT: return &out_left;
        case output::OUT_RIGHT: return &out_right;
        default: return 0;
    }
}

const void*
fast_lookahead_limiter::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_LEFT:  return in_left = (double*)o;
        case input::IN_RIGHT: return in_right = (double*)o;
        default: return 0;
    }
}

const void* fast_lookahead_limiter::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_LEFT:  return in_left;
        case input::IN_RIGHT: return in_right;
        default: return 0;
    }
}

bool
fast_lookahead_limiter::set_param(param::TYPE pt,
                                  const void* data)
{
    switch(pt)
    {
        case param::GAIN_DB:
            gain_db = *(double*)data;
            return true;
        case param::LIMIT_DB:
            limit_db = *(double*)data;
            return true;
        case param::RELEASE_SECS:
            release_secs = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void*
fast_lookahead_limiter::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::GAIN_DB:      return &gain_db;
        case param::LIMIT_DB:     return &limit_db;
        case param::RELEASE_SECS: return &release_secs;
        default: return 0;
    }
}

errors::TYPE fast_lookahead_limiter::validate()
{
    if (gain_db < -20 || gain_db > 20) {
        sm_err("%s must be within range -20.0 ~ +20.0.",
                param::names::get(param::GAIN_DB));
        invalidate();
        return errors::ERROR;
    }
    if (limit_db < -20 || limit_db > 0) {
        sm_err("%s must be within range -20.0 ~ 0.0.",
                param::names::get(param::LIMIT_DB));
        invalidate();
        return errors::ERROR;
    }
    if (release_secs < 0.01 || release_secs > 2.0) {
        sm_err("%s must be within range 0.01 ~ 2.0.",
                param::names::get(param::RELEASE_SECS));
        invalidate();
        return errors::ERROR;
    }
    return errors::NO_ERROR;
}

void fast_lookahead_limiter::init()
{
    ladspa_loader* ll = wcnt::jwm.get_ladspaloader();
    ladspa_plug* lp = ll->get_plugin("fast_lookahead_limiter_1913",
                                     "fastLookaheadLimiter");
    if (lp == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return;
    }
    if ((l_descriptor = lp->get_descriptor()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return;
    }
    if ((l_inst_handle = lp->instantiate()) == 0) {
        sm_err("%s", ladspa_loader::get_error_msg());
        invalidate();
        return;
    }
    l_in_left = new LADSPA_Data[1];
    l_in_right = new LADSPA_Data[1];
    l_out_left = new LADSPA_Data[1];
    l_out_right = new LADSPA_Data[1];
    l_descriptor->connect_port(l_inst_handle, 0, &l_gain_db);
    l_descriptor->connect_port(l_inst_handle, 1, &l_limit_db);
    l_descriptor->connect_port(l_inst_handle, 2, &l_release_secs);
    l_descriptor->connect_port(l_inst_handle, 3, &l_out_atten);
    l_descriptor->connect_port(l_inst_handle, 4, l_in_left);
    l_descriptor->connect_port(l_inst_handle, 5, l_in_right);
    l_descriptor->connect_port(l_inst_handle, 6, l_out_left);
    l_descriptor->connect_port(l_inst_handle, 7, l_out_right);
    l_descriptor->connect_port(l_inst_handle, 8, &l_out_latency);
    l_descriptor->activate(l_inst_handle);
}

void fast_lookahead_limiter::run()
{
    *l_in_left  = *in_left;
    *l_in_right = *in_right;
    l_gain_db = gain_db;
    l_limit_db = limit_db;
    l_release_secs = release_secs;
    l_descriptor->run(l_inst_handle, 1);
    out_left = *l_out_left;
    out_right = *l_out_right;
}


#endif // WITH_LADSPA
