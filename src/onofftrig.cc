
#include "../include/onofftrig.h"
#include "../include/conversions.h"

#include <math.h>

onofftrig::onofftrig(const char* uname) :
 synthmod::base(synthmod::ONOFFTRIG, uname, SM_HAS_OUT_TRIG),
 in_signal(0), out_trig(OFF), out_not_trig(OFF), out_attack_state(OFF),
 out_release_state(OFF), attack_time(0.0), release_time(0.0),
 attack_level(0.0), release_level(0.0), check_levels(OFF), 
 attack_samps(0), release_samps(0), do_attack(true)
{
    register_output(output::OUT_TRIG);
    register_output(output::OUT_NOT_TRIG);
    register_output(output::OUT_ATTACK_STATE);
    register_output(output::OUT_RELEASE_STATE);
}

void onofftrig::register_ui()
{
    register_input(input::IN_SIGNAL);
    register_param(param::ATTACK_TIME);
    register_param(param::ATTACK_LEVEL);
    register_param(param::RELEASE_TIME);
    register_param(param::RELEASE_LEVEL);
    register_param(param::CHECK_LEVELS);
}

onofftrig::~onofftrig()
{
}

const void* onofftrig::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_TRIG:
            return &out_trig;
        case output::OUT_NOT_TRIG:
            return &out_not_trig;
        case output::OUT_ATTACK_STATE:
            return &out_attack_state;
        case output::OUT_RELEASE_STATE:
            return &out_release_state;
        default: return 0;
    }
}

const void* onofftrig::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal = (double*)o;
        default: return 0;
    }
}

const void* onofftrig::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_SIGNAL: return in_signal;
        default: return 0;
    }
}

bool onofftrig::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::ATTACK_TIME:
            attack_time = *(double*)data;
            return true;
        case param::RELEASE_TIME:
            release_time = *(double*)data;
            return true;
        case param::ATTACK_LEVEL:
            attack_level = *(double*)data;
            return true;
        case param::RELEASE_LEVEL:
            release_level = *(double*)data;
            return true;
        case param::CHECK_LEVELS:
            check_levels = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

const void* onofftrig::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::ATTACK_TIME:   return &attack_time;
        case param::RELEASE_TIME:  return &release_time;
        case param::ATTACK_LEVEL:  return &attack_level;
        case param::RELEASE_LEVEL: return &release_level;
        case param::CHECK_LEVELS:  return &check_levels;
        default: return 0;
    }
}

errors::TYPE onofftrig::validate()
{
    if (!validate_param(param::ATTACK_TIME, errors::NEG_OR_ZERO))
        return errors::NEG_OR_ZERO;

    if (!validate_param(param::ATTACK_LEVEL, errors::NEG_OR_ZERO))
        return errors::NEG_OR_ZERO;

    if (!validate_param(param::RELEASE_TIME, errors::NEG_OR_ZERO))
        return errors::NEG_OR_ZERO;

    if (!validate_param(param::RELEASE_LEVEL, errors::NEG_OR_ZERO))
        return errors::NEG_OR_ZERO;

    if (check_levels == ON && attack_level <= release_level) {
        sm_err("When %s is set on %s must be higher than %s.",
                        param::names::get(param::CHECK_LEVELS),
                        param::names::get(param::ATTACK_LEVEL),
                        param::names::get(param::RELEASE_LEVEL));
        invalidate();
        return errors::ERROR;
    }
    return errors::NO_ERROR;
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

