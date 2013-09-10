#ifndef ONOFFTRIG_H
#include "../include/onofftrig.h"

onofftrig::onofftrig(char const* uname) :
 synthmod(synthmodnames::MOD_ONOFFTRIG, onofftrig_count, uname),
 in_signal(0), out_trig(OFF), out_not_trig(OFF), out_attack_state(OFF),
 out_release_state(OFF), attack_time(0.0), release_time(0.0),
 attack_level(0.0), release_level(0.0), check_levels(OFF), 
 attack_samps(0), release_samps(0), do_attack(true)
{
    get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_NOT_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_ATTACK_STATE);
    get_outputlist()->add_output(this, outputnames::OUT_RELEASE_STATE);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
    onofftrig_count++;
    create_params();
}

onofftrig::~onofftrig()
{
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
}

void const* onofftrig::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_TRIG:
        o = &out_trig;
        break;
    case outputnames::OUT_NOT_TRIG:
        o = &out_not_trig;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* onofftrig::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_SIGNAL:
        i = in_signal = (double*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool onofftrig::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_ATTACK_TIME:
        set_attack_time(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_RELEASE_TIME:
        set_release_time(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_ATTACK_LEVEL:
        set_attack_level(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_RELEASE_LEVEL:
        set_release_level(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_CHECK_LEVELS:
        set_check_levels(*(STATUS*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* onofftrig::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_ATTACK_TIME:
        return &attack_time;
    case paramnames::PAR_RELEASE_TIME:
        return &release_time;
    case paramnames::PAR_ATTACK_LEVEL:
        return &attack_level;
    case paramnames::PAR_RELEASE_LEVEL:
        return &release_level;
    case paramnames::PAR_CHECK_LEVELS:
        return &check_levels;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE onofftrig::validate()
{
    modparamlist* pl = get_paramlist();
    paramnames* pns = get_paramnames();
    if (!pl->validate(this, paramnames::PAR_ATTACK_TIME,
            stockerrs::ERR_NEG_ZERO))
    {
        *err_msg = pns->get_name(paramnames::PAR_ATTACK_TIME);
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    if (!pl->validate(this, paramnames::PAR_ATTACK_LEVEL,
            stockerrs::ERR_NEG_ZERO))
    {
        *err_msg = pns->get_name(paramnames::PAR_ATTACK_LEVEL);
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    if (!pl->validate(this, paramnames::PAR_RELEASE_TIME,
            stockerrs::ERR_NEG_ZERO))
    {
        *err_msg = pns->get_name(paramnames::PAR_RELEASE_TIME);
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    if (!pl->validate(this, paramnames::PAR_RELEASE_LEVEL,
            stockerrs::ERR_NEG_ZERO))
    {
        *err_msg = pns->get_name(paramnames::PAR_RELEASE_LEVEL);
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    if (check_levels == ON && attack_level <= release_level) {
        *err_msg = "when ";
        *err_msg += pns->get_name(paramnames::PAR_CHECK_LEVELS);
        *err_msg += " is set on, ";
        *err_msg += pns->get_name(paramnames::PAR_ATTACK_LEVEL);
        *err_msg += " must be higher than ";
        *err_msg += pns->get_name(paramnames::PAR_RELEASE_LEVEL);
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    return stockerrs::ERR_NO_ERROR;
}

void onofftrig::run()
{
    double fs = fabs(*in_signal);
    if (out_attack_state == OFF && out_release_state == OFF)
    {
        if (do_attack) {
            if (fs > attack_level) {
                out_trig = ON;
                attack_samps = ms_to_samples(attack_time);
                out_attack_state = ON;
                do_attack = false;
            }
        }
        else {
            if (fs < release_level) {
                out_not_trig = ON;
                release_samps = ms_to_samples(release_time);
                out_release_state = ON;
                do_attack = true;
            }
        }
    }
    else {
        if (out_trig == ON)
            out_trig = OFF;
        if (out_not_trig == ON)
            out_not_trig = OFF;
        if (out_attack_state == ON) {
            if (attack_samps == 0) {
                if (fs < attack_level || check_levels == OFF)
                    out_attack_state = OFF;
            }
            else
                attack_samps--;
        }
        if (out_release_state == ON) {
            if (release_samps == 0) {
                if (fs > release_level || check_levels == OFF)
                    out_release_state = OFF;
            }
            else
                release_samps--;
        }
    }
}

int onofftrig::onofftrig_count = 0;

bool onofftrig::done_params = false;

void onofftrig::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_ONOFFTRIG, paramnames::PAR_ATTACK_TIME);
    get_paramlist()->add_param(
     synthmodnames::MOD_ONOFFTRIG, paramnames::PAR_ATTACK_LEVEL);
    get_paramlist()->add_param(
     synthmodnames::MOD_ONOFFTRIG, paramnames::PAR_RELEASE_TIME);
    get_paramlist()->add_param(
     synthmodnames::MOD_ONOFFTRIG, paramnames::PAR_RELEASE_LEVEL);
    get_paramlist()->add_param(
     synthmodnames::MOD_ONOFFTRIG, paramnames::PAR_CHECK_LEVELS);
    done_params = true;
}
#endif