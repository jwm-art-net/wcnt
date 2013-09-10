#ifndef LFOCONTROL_H
#include "../include/lfocontroller.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

lfo_controller::lfo_controller(char const* uname) :
 synthmod(synthmodnames::LFOCONTROL, uname, SM_HAS_OUT_OUTPUT),
 in_trig(0), in_wave(0), in_amp_mod(0), out_preampmod(0), output(0),
 delay_time(0), ramp_time(0), start_level(0), end_level(0),
 response_time(0), amp_modsize(0), ams_r(0), delay_samples(0),
 ramp_samples(0), resp_size(0), resp_fa_level(0), resp_ac(0),
 level_size(0), current_level(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_PRE_AMP_MOD);
    jwm.get_inputlist()->add_input(this, inputnames::IN_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_WAVE);
    jwm.get_inputlist()->add_input(this, inputnames::IN_AMP_MOD);
    create_params();
}

lfo_controller::~lfo_controller()
{
}

void const* lfo_controller::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT:       return &output;
        case outputnames::OUT_PRE_AMP_MOD:  return &out_preampmod;
        default: return 0;
    }
}

void const* lfo_controller::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_TRIG:       return in_trig = (STATUS*)o;
        case inputnames::IN_WAVE:       return in_wave = (double*)o;
        case inputnames::IN_AMP_MOD:    return in_amp_mod = (double*)o;
        default: return 0;
    }
}

void const* lfo_controller::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_TRIG:   return in_trig;
        case inputnames::IN_WAVE:   return in_wave;
        case inputnames::IN_AMP_MOD:return in_amp_mod;
        default: return 0;
    }
}

bool lfo_controller::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::DELAY_TIME:
            delay_time = *(double*)data;
            return true;
        case paramnames::RAMP_TIME:
            ramp_time = *(double*)data;
            return true;
        case paramnames::START_LEVEL:
            start_level = *(double*)data;
            return true;
        case paramnames::END_LEVEL:
            end_level = *(double*)data;
            return true;
        case paramnames::RESPONSE_TIME:
            response_time = *(double*)data;
            return true;
        case paramnames::AMP_MODSIZE:
            amp_modsize = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* lfo_controller::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::DELAY_TIME:    return &delay_time;
        case paramnames::RAMP_TIME:     return &ramp_time;
        case paramnames::START_LEVEL:   return &start_level;
        case paramnames::END_LEVEL:     return &end_level;
        case paramnames::RESPONSE_TIME: return &response_time;
        case paramnames::AMP_MODSIZE:   return &amp_modsize;
        default: return 0;
    }
}

stockerrs::ERR_TYPE lfo_controller::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::DELAY_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::DELAY_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::RAMP_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::RAMP_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::RESPONSE_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::RESPONSE_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (response_time < 0) {
        *err_msg
         += jwm.get_paramnames()->get_name(paramnames::RESPONSE_TIME);
        *err_msg += " less than zero";
        invalidate();
    }
    return stockerrs::ERR_NO_ERROR;
}

void lfo_controller::init()
{
    resp_size = (double) 1 / ms_to_samples(response_time);
    ams_r = 1 - amp_modsize;
}

void lfo_controller::run()
{
    if (*in_trig == ON)	{
        resp_fa_level = output;
        resp_ac = 1;
        current_level = start_level;
        delay_samples = ms_to_samples(delay_time);
        ramp_samples = ms_to_samples(ramp_time);
        level_size = (end_level - start_level) / ramp_samples;
    }
    else if (delay_samples > 0)
        delay_samples--;
    else if (ramp_samples > 0) {
        current_level += level_size;
        ramp_samples--;
    }
    if (resp_ac > 0) {
        // out_preampmod = *in_wave * resp_fa_level * resp_ac
        // + *in_wave * current_level * (1 - resp_ac);
        out_preampmod = resp_fa_level * resp_ac
                        + *in_wave * current_level * (1 - resp_ac);
        resp_ac -= resp_size;
    }
    else
        out_preampmod = *in_wave * current_level;
    if (out_preampmod > 1)
        out_preampmod = 1;
    else if (out_preampmod < -1)
        out_preampmod = -1;
    output = 
     out_preampmod * ams_r + out_preampmod * *in_amp_mod * amp_modsize;
}

bool lfo_controller::done_params = false;

void lfo_controller::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::LFOCONTROL, paramnames::DELAY_TIME);
    jwm.get_paramlist()->add_param(
        synthmodnames::LFOCONTROL, paramnames::RAMP_TIME);
    jwm.get_paramlist()->add_param(
        synthmodnames::LFOCONTROL, paramnames::START_LEVEL);
    jwm.get_paramlist()->add_param(
        synthmodnames::LFOCONTROL, paramnames::END_LEVEL);
    jwm.get_paramlist()->add_param(
        synthmodnames::LFOCONTROL, paramnames::RESPONSE_TIME);
    jwm.get_paramlist()->add_param(
        synthmodnames::LFOCONTROL, paramnames::AMP_MODSIZE);
    done_params = true;
}

#endif
