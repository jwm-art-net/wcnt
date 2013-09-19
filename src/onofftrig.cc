
#include "../include/onofftrig.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

#include <math.h>

onofftrig::onofftrig(char const* uname) :

 synthmod(
    synthmodnames::ONOFFTRIG,
    uname,
    SM_HAS_OUT_TRIG),

 in_signal(0), out_trig(OFF), out_not_trig(OFF), out_attack_state(OFF),
 out_release_state(OFF), attack_time(0.0), release_time(0.0),
 attack_level(0.0), release_level(0.0), check_levels(OFF), 
 attack_samps(0), release_samps(0), do_attack(true)
{
    register_input(inputnames::IN_SIGNAL);
    register_output(outputnames::OUT_TRIG);
    register_output(outputnames::OUT_NOT_TRIG);
    register_output(outputnames::OUT_ATTACK_STATE);
    register_output(outputnames::OUT_RELEASE_STATE);
    init_first();
}

onofftrig::~onofftrig()
{
}

void const* onofftrig::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_TRIG:
            return &out_trig;
        case outputnames::OUT_NOT_TRIG:
            return &out_not_trig;
        case outputnames::OUT_ATTACK_STATE:
            return &out_attack_state;
        case outputnames::OUT_RELEASE_STATE:
            return &out_release_state;
        default: return 0;
    }
}

void const* onofftrig::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

void const* onofftrig::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool onofftrig::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::ATTACK_TIME:
            attack_time = *(double*)data;
            return true;
        case paramnames::RELEASE_TIME:
            release_time = *(double*)data;
            return true;
        case paramnames::ATTACK_LEVEL:
            attack_level = *(double*)data;
            return true;
        case paramnames::RELEASE_LEVEL:
            release_level = *(double*)data;
            return true;
        case paramnames::CHECK_LEVELS:
            check_levels = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

void const* onofftrig::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::ATTACK_TIME:   return &attack_time;
        case paramnames::RELEASE_TIME:  return &release_time;
        case paramnames::ATTACK_LEVEL:  return &attack_level;
        case paramnames::RELEASE_LEVEL: return &release_level;
        case paramnames::CHECK_LEVELS:  return &check_levels;
        default: return 0;
    }
}

stockerrs::ERR_TYPE onofftrig::validate()
{
    modparamlist* pl = jwm.get_paramlist();
    const paramnames* pns = jwm.get_paramnames();
    if (!pl->validate(this, paramnames::ATTACK_TIME,
            stockerrs::ERR_NEG_ZERO))
    {
        sm_err("%s", pns->get_name(paramnames::ATTACK_TIME));
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    if (!pl->validate(this, paramnames::ATTACK_LEVEL,
            stockerrs::ERR_NEG_ZERO))
    {
        sm_err("%s", pns->get_name(paramnames::ATTACK_LEVEL));
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    if (!pl->validate(this, paramnames::RELEASE_TIME,
            stockerrs::ERR_NEG_ZERO))
    {
        sm_err("%s", pns->get_name(paramnames::RELEASE_TIME));
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    if (!pl->validate(this, paramnames::RELEASE_LEVEL,
            stockerrs::ERR_NEG_ZERO))
    {
        sm_err("%s", pns->get_name(paramnames::RELEASE_LEVEL));
        invalidate();
        return stockerrs::ERR_NEG_ZERO;
    }
    if (check_levels == ON && attack_level <= release_level) {
        sm_err("When %s is set on %s must be higher than %s.",
                        pns->get_name(paramnames::CHECK_LEVELS),
                        pns->get_name(paramnames::ATTACK_LEVEL),
                        pns->get_name(paramnames::RELEASE_LEVEL));
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

void onofftrig::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::ATTACK_TIME);
    register_param(paramnames::ATTACK_LEVEL);
    register_param(paramnames::RELEASE_TIME);
    register_param(paramnames::RELEASE_LEVEL);
    register_param(paramnames::CHECK_LEVELS);
}
