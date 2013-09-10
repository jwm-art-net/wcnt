#ifndef TRIGGER_H
#include "../include/trigger.h"

trigger::trigger(char const* uname) :
 synthmod(synthmodnames::MOD_TRIGGER, trigger_count, uname),
 in_signal(0), out_trig(OFF), out_not_trig(OFF),
 attack_time(0.0), release_time(0.0), attack_level(0.0),
 release_level(0.0), ind_mode(OFF), attack_samps(0), release_samps(0)
{
#ifndef BARE_MODULES
    get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_NOT_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_SIGNAL);
#endif
    trigger_count++;
#ifndef BARE_MODULES
    create_params();
#endif
}

trigger::~trigger()
{
#ifndef BARE_MODULES
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
#endif
}

#ifndef BARE_MODULES
void const* trigger::get_out(outputnames::OUT_TYPE ot)
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

void const* trigger::set_in(inputnames::IN_TYPE it, void const* o)
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

bool trigger::set_param(paramnames::PAR_TYPE pt, void const* data)
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
    case paramnames::PAR_INDEPENDANT:
        set_independant_mode(*(STATUS*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* trigger::get_param(paramnames::PAR_TYPE pt)
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
    case paramnames::PAR_INDEPENDANT:
        return &ind_mode;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE trigger::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_ATTACK_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_ATTACK_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::PAR_ATTACK_LEVEL,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_ATTACK_LEVEL);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::PAR_RELEASE_TIME,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_RELEASE_TIME);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::PAR_RELEASE_LEVEL,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_RELEASE_LEVEL);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

#endif

void trigger::run()
{
    double fs = fabs(*in_signal);
    if (attack_samps == 0) {
        if (fs >= attack_level) {
            attack_samps = ms_to_samples(attack_time);
            out_trig = ON;
        }
    }
    else {
        if (out_trig == ON) out_trig = OFF;
        attack_samps--;
    }
    if (release_samps == 0) {
        if (fs < release_level) {
            if ((ind_mode == OFF && attack_samps == 0) || ind_mode == ON)
            {
                release_samps = ms_to_samples(release_time);
                out_not_trig = ON;
            }
        }
    }
    else {
        if (out_not_trig == ON) out_not_trig = OFF;
        release_samps--;
    }
}

int trigger::trigger_count = 0;

#ifndef BARE_MODULES
bool trigger::done_params = false;

void trigger::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_TRIGGER, paramnames::PAR_ATTACK_TIME);
    get_paramlist()->add_param(
     synthmodnames::MOD_TRIGGER, paramnames::PAR_ATTACK_LEVEL);
    get_paramlist()->add_param(
     synthmodnames::MOD_TRIGGER, paramnames::PAR_RELEASE_TIME);
    get_paramlist()->add_param(
     synthmodnames::MOD_TRIGGER, paramnames::PAR_RELEASE_LEVEL);
    get_paramlist()->add_param(
     synthmodnames::MOD_TRIGGER, paramnames::PAR_INDEPENDANT);
    done_params = true;
}
#endif
#endif
