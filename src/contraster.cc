#ifndef CONTRASTER_H
#include "../include/contraster.h"

contraster::contraster(char const* uname) :
 synthmod(synthmodnames::MOD_CONTRASTER, contraster_count, uname),
 out_output(0.0), in_signal(0), in_power_mod(0),
 power_min(1.0), power_max(2.0), rude_mode(OFF),
 power(0.0), powerrad(0.0)
{
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    get_inputlist()->add_input(this, inputnames::IN_POWER_MOD);
    get_inputlist()->add_input(this, inputnames::IN_RUDE_SWITCH_TRIG);
    contraster_count++;
    create_params();
}

contraster::~contraster()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* contraster::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_OUTPUT:
        o = &out_output;
        break;
    default:
        o = 0;
    }
    return o;
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

void const* contraster::get_in(inputnames::IN_TYPE it)
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
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_POWER_MIN:
        set_power_min(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_POWER_MAX:
        set_power_max(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_RUDE_MODE:
        set_rude_mode(*(STATUS*)data);
        retv = true;
        break;
    case paramnames::PAR_WETDRY:
        set_wetdry(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* contraster::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_POWER_MIN:
        return &power_min;
    case paramnames::PAR_POWER_MAX:
        return &power_max;
    case paramnames::PAR_RUDE_MODE:
        return &rude_mode;
    case paramnames::PAR_WETDRY:
        return &wetdry;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE contraster::validate()
{
    if (!get_paramlist()->validate(this, paramnames::PAR_WETDRY,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_WETDRY);
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

int contraster::contraster_count = 0;

bool contraster::done_params = false;

void contraster::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_CONTRASTER, paramnames::PAR_POWER_MIN);
    get_paramlist()->add_param(
     synthmodnames::MOD_CONTRASTER, paramnames::PAR_POWER_MAX);
    get_paramlist()->add_param(
     synthmodnames::MOD_CONTRASTER, paramnames::PAR_RUDE_MODE);
    get_paramlist()->add_param(
     synthmodnames::MOD_CONTRASTER, paramnames::PAR_WETDRY);
    done_params = true;
}

#endif
