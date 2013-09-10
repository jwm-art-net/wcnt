#ifndef WAITTRIG_H
#include "../include/waittrig.h"

waittrig::waittrig(char const* uname) :
 synthmod(synthmodnames::MOD_WAITTRIG, waittrig_count, uname),
 in_trig1(0), in_trig2(0), out_trig(OFF), min_time(0), max_time(0),
 min_samples(0), max_samples(0), mins(0), maxs(0), trig_pending(OFF)
{
#ifndef BARE_MODULES
    get_outputlist()->add_output(this, outputnames::OUT_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_TRIG1);
    get_inputlist()->add_input(this, inputnames::IN_TRIG2);
#endif
    waittrig_count++;
#ifndef BARE_MODULES
    create_params();
#endif
}

waittrig::~waittrig()
{
#ifndef BARE_MODULES
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
#endif
}

void waittrig::set_min_time(double t)
{
    min_time = t;
    mins = min_samples = ms_to_samples(min_time);
}

void waittrig::set_max_time(double t)
{
    max_time = t;
    maxs = max_samples = ms_to_samples(max_time);
}


#ifndef BARE_MODULES
void const* waittrig::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_TRIG:
        o = &out_trig;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* waittrig::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_TRIG1:
        i = in_trig1 = (STATUS*)o;
        break;
    case inputnames::IN_TRIG2:
        i = in_trig2 = (STATUS*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool waittrig::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_MIN_WAIT:
        set_min_time(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_MAX_WAIT:
        set_max_time(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* waittrig::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_MIN_WAIT:
        return &min_time;
    case paramnames::PAR_MAX_WAIT:
        return &max_time;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE waittrig::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_MIN_WAIT,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_MIN_WAIT);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::PAR_MAX_WAIT,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_MAX_WAIT);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

#endif // BARE_MODULES

void waittrig::run()
{
    if (trig_pending == OFF) {
        if (mins == 0) {
            if (*in_trig2 == ON) {
                trig_pending = ON;
                mins = min_samples;
                maxs = max_samples;
            }
            if (maxs == 0 && max_samples > 0) {
                trig_pending = ON;
                mins = min_samples;
                maxs = max_samples;
            }
            else if (maxs > 0) maxs--;
        }
        else mins--;
        if (out_trig == ON) out_trig = OFF;
    }
    else {
        if (*in_trig1 == ON) {
            out_trig = ON;
            trig_pending = OFF;
        }
    }
}

int waittrig::waittrig_count = 0;

#ifndef BARE_MODULES
bool waittrig::done_params = false;

void waittrig::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_WAITTRIG, paramnames::PAR_MIN_WAIT);
    get_paramlist()->add_param(
     synthmodnames::MOD_WAITTRIG, paramnames::PAR_MAX_WAIT);
    done_params = true;
}
#endif
#endif
