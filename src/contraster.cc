#ifndef CONTRASTER_H
#include "../include/contraster.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

contraster::contraster(char const* uname) :
 synthmod(synthmodnames::CONTRASTER, uname, SM_HAS_OUT_OUTPUT),
 out_output(0.0), in_signal(0), in_power_mod(0),
 power_min(1.0), power_max(2.0), rude_mode(OFF),
 power(0.0), powerrad(0.0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    jwm.get_inputlist()->add_input(this, inputnames::IN_POWER_MOD);
    jwm.get_inputlist()->add_input(this, inputnames::IN_RUDE_SWITCH_TRIG);
    create_params();
}

contraster::~contraster()
{
}

void const* contraster::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

void const* contraster::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:
            return in_signal = (double*)o;
        case inputnames::IN_POWER_MOD:
            return in_power_mod = (double*)o;
        case inputnames::IN_RUDE_SWITCH_TRIG:
            return in_rude_switch_trig = (STATUS*)o;
        default:
            return 0;
    }
}

void const* contraster::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL:
            return in_signal;
        case inputnames::IN_POWER_MOD:
            return in_power_mod;
        case inputnames::IN_RUDE_SWITCH_TRIG:
            return in_rude_switch_trig;
        default:
            return 0;
    }
}

bool contraster::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::POWER_MIN:
            power_min = *(double*)data;
            return true;
        case paramnames::POWER_MAX:
            power_max = *(double*)data;
            return true;
        case paramnames::RUDE_MODE:
            rude_mode = *(STATUS*)data;
            return true;
        case paramnames::WETDRY:
            wetdry = *(double*)data;
            return true;
        default:
            return false;
    }
}

void const* contraster::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
    case paramnames::POWER_MIN:
        return &power_min;
    case paramnames::POWER_MAX:
        return &power_max;
    case paramnames::RUDE_MODE:
        return &rude_mode;
    case paramnames::WETDRY:
        return &wetdry;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE contraster::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::WETDRY,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::WETDRY);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    return stockerrs::ERR_NO_ERROR;
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

bool contraster::done_params = false;

void contraster::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(
     synthmodnames::CONTRASTER, paramnames::POWER_MIN);
    jwm.get_paramlist()->add_param(
     synthmodnames::CONTRASTER, paramnames::POWER_MAX);
    jwm.get_paramlist()->add_param(
     synthmodnames::CONTRASTER, paramnames::RUDE_MODE);
    jwm.get_paramlist()->add_param(
     synthmodnames::CONTRASTER, paramnames::WETDRY);
    done_params = true;
}

#endif
