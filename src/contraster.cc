#include "../include/contraster.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

contraster::contraster(const char* uname) :
 synthmod(synthmodnames::CONTRASTER, uname, SM_HAS_OUT_OUTPUT),
 out_output(0.0), in_signal(0), in_power_mod(0),
 in_rude_switch_trig(0), power_min(1.0), power_max(2.0), rude_mode(OFF),
 wetdry(0), power(0.0), power_mod(0), powerrad(0.0), output(0)
{
    register_input(inputnames::IN_SIGNAL);
    register_input(inputnames::IN_POWER_MOD);
    register_input(inputnames::IN_RUDE_SWITCH_TRIG);
    register_output(outputnames::OUT_OUTPUT);
    init_first();
}

contraster::~contraster()
{
}

const void* contraster::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT: return &out_output;
        default: return 0;
    }
}

const void* contraster::set_in(inputnames::IN_TYPE it, const void* o)
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

const void* contraster::get_in(inputnames::IN_TYPE it) const
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

bool contraster::set_param(paramnames::PAR_TYPE pt, const void* data)
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

const void* contraster::get_param(paramnames::PAR_TYPE pt) const
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
        sm_err("%s", paramnames::get_name(paramnames::WETDRY));
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



void contraster::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::POWER_MIN);
    register_param(paramnames::POWER_MAX);
    register_param(paramnames::RUDE_MODE);
    register_param(paramnames::WETDRY);
}

