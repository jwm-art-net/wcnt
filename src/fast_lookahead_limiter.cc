#include "../include/fast_lookahead_limiter.h"
#ifdef WITH_LADSPA
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

fast_lookahead_limiter::fast_lookahead_limiter(char const* uname) :

 synthmod(
    synthmodnames::FAST_LOOKAHEAD_LIMITER,
    uname,
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
    jwm.get_outputlist()->add_output(this, outputnames::OUT_LEFT);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_RIGHT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_LEFT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_RIGHT);
    create_params();
}

fast_lookahead_limiter::~fast_lookahead_limiter()
{
    if (l_descriptor) l_descriptor->cleanup(l_inst_handle);
    if (l_in_left)  delete [] l_in_left;
    if (l_in_right) delete [] l_in_right;
    if (l_out_left) delete [] l_out_left;
    if (l_out_right)delete [] l_out_right;
}

void const*
fast_lookahead_limiter::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_LEFT: return &out_left;
        case outputnames::OUT_RIGHT: return &out_right;
        default: return 0;
    }
}

void const*
fast_lookahead_limiter::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_LEFT:  return in_left = (double*)o;
        case inputnames::IN_RIGHT: return in_right = (double*)o;
        default: return 0;
    }
}

void const* fast_lookahead_limiter::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_LEFT:  return in_left;
        case inputnames::IN_RIGHT: return in_right;
        default: return 0;
    }
}

bool
fast_lookahead_limiter::set_param(paramnames::PAR_TYPE pt,
                                  void const* data)
{
    switch(pt)
    {
        case paramnames::GAIN_DB:
            gain_db = *(double*)data;
            return true;
        case paramnames::LIMIT_DB:
            limit_db = *(double*)data;
            return true;
        case paramnames::RELEASE_SECS:
            release_secs = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const*
fast_lookahead_limiter::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::GAIN_DB:      return &gain_db;
        case paramnames::LIMIT_DB:     return &limit_db;
        case paramnames::RELEASE_SECS: return &release_secs;
        default: return 0;
    }
}

stockerrs::ERR_TYPE fast_lookahead_limiter::validate()
{
    if (gain_db < -20 || gain_db > 20) {
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::GAIN_DB);
        *err_msg += " must be within range -20.0 to +20.0";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (limit_db < -20 || limit_db > 0) {
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::LIMIT_DB);
        *err_msg += " must be within range -20.0 to 0.0";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (release_secs < 0.01 || release_secs > 2.0) {
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::RELEASE_SECS);
        *err_msg += " must be within range 0.01 to 2.0";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

void fast_lookahead_limiter::init()
{
    ladspa_loader* ll = jwm.get_ladspaloader();
    ladspa_plug* lp = ll->get_plugin("fast_lookahead_limiter_1913",
                                     "fastLookaheadLimiter");
    if (lp == 0) {
        *err_msg = ll->get_error_msg();
        invalidate();
        return;
    }
    if ((l_descriptor = lp->get_descriptor()) == 0) {
        *err_msg = lp->get_error_msg();
        invalidate();
        return;
    }
    if ((l_inst_handle = lp->instantiate()) == 0) {
        *err_msg = lp->get_error_msg();
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

void fast_lookahead_limiter::create_params()
{
    if (done_params())
        return;
    jwm.get_paramlist()->add_param(synthmodnames::FAST_LOOKAHEAD_LIMITER,
                               paramnames::GAIN_DB);
    jwm.get_paramlist()->add_param(synthmodnames::FAST_LOOKAHEAD_LIMITER,
                               paramnames::LIMIT_DB);
    jwm.get_paramlist()->add_param(synthmodnames::FAST_LOOKAHEAD_LIMITER,
                               paramnames::RELEASE_SECS);
}

#endif // WITH_LADSPA
