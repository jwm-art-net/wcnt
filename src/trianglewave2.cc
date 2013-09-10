#ifndef TRIANGLEWAVE2_H
#include "../include/trianglewave2.h"

triangle_wave2::triangle_wave2(char const* uname) :
 synthmod(synthmodnames::MOD_TRIWAVE2, triangle_wave2_count, uname),
 in_phase_trig(0), in_deg_size(0), in_normal_mod(0), output(0.00),
 play_state(OFF), normal_freq(440.00), norm_modsize(0), recycle(OFF),
 zero_retrigger_mode(OFF), nf_deg_size(0), nf_pre_deg_size(0),
 nf_sectmaxsamps(0), nf_sectsample(0), nf_counter_ratio(0.0), sect(0),
 sect_spanlvl(0), sect_startlvl(0), old_maxsamps(0), sectmaxsamps(1),
 sectsample(0), counter_ratio(0)
{
#ifndef BARE_MODULES
    get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE);
    get_inputlist()->add_input(this, inputnames::IN_PHASE_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_DEG_SIZE);
    get_inputlist()->add_input(this, inputnames::IN_NORM_MOD);
#endif
    triangle_wave2_count++;
#ifndef BARE_MODULES
    create_params();
#endif
}

triangle_wave2::~triangle_wave2()
{
#ifndef BARE_MODULES
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
#endif
}

#ifndef BARE_MODULES
void const* triangle_wave2::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_OUTPUT:
        o = &output;
        break;
    case outputnames::OUT_PLAY_STATE:
        o = &play_state;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* triangle_wave2::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_PHASE_TRIG:
        i = in_phase_trig = (STATUS*)o;
        break;
    case inputnames::IN_DEG_SIZE:
        i = in_deg_size = (double*)o;
        break;
    case inputnames::IN_NORM_MOD:
        i = in_normal_mod = (double*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool triangle_wave2::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_NORM_FREQ:
        set_normal_frequency(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_NORM_MODSIZE:
        set_normal_modsize(*(double*)data);
        retv = true;
        break;
    case paramnames::PAR_RECYCLE_MODE:
        set_recycle_mode(*(STATUS*)data);
        retv = true;
        break;
    case paramnames::PAR_ZERO_RETRIGGER:
        set_zero_retrigger_mode(*(STATUS*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* triangle_wave2::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_NORM_FREQ:
        return &normal_freq;
    case paramnames::PAR_NORM_MODSIZE:
        return &norm_modsize;
    case paramnames::PAR_RECYCLE_MODE:
        return &recycle;
    case paramnames::PAR_ZERO_RETRIGGER:
        return &zero_retrigger_mode;
    default:
        return 0;
    }
}

stockerrs::ERR_TYPE triangle_wave2::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_NORM_FREQ,
            stockerrs::ERR_RANGE_FREQ))
    {
        *err_msg = get_paramnames()->get_name(paramnames::PAR_NORM_FREQ);
        invalidate();
        return stockerrs::ERR_RANGE_FREQ;
    }
    if (!pl->validate(this, paramnames::PAR_NORM_MODSIZE,
            stockerrs::ERR_RANGE_FMOD))
    {
        *err_msg
         = get_paramnames()->get_name(paramnames::PAR_NORM_MODSIZE);
        invalidate();
        return stockerrs::ERR_RANGE_FMOD;
    }
    return stockerrs::ERR_NO_ERROR;
}

#endif

void triangle_wave2::init()
{
    nf_pre_deg_size = freq_to_step(normal_freq, 0);
}

void triangle_wave2::run()
{
    if (*in_phase_trig == ON)
    {
        play_state = ON;
        sect = 0;
        if (zero_retrigger_mode == ON)
            output = 0;
        sect_startlvl = output;
        sect_spanlvl = 1 - sect_startlvl;
        sectsample = 0;
        nf_sectsample = 0;
    }
    if (play_state == ON)
    {
        if (*in_normal_mod < 0)
            nf_deg_size = nf_pre_deg_size
             / (1 + norm_modsize * -*in_normal_mod);
        else
            nf_deg_size = nf_pre_deg_size
             * (1 + norm_modsize * *in_normal_mod);
        nf_counter_ratio = nf_sectsample / nf_sectmaxsamps;
        nf_sectmaxsamps = 90 / nf_deg_size;
        nf_sectsample = nf_sectmaxsamps * nf_counter_ratio;
        nf_sectsample++;
        if (nf_sectsample < nf_sectmaxsamps)
            output = sect_startlvl + sect_spanlvl * nf_counter_ratio;
        counter_ratio = sectsample / sectmaxsamps;
        sectmaxsamps = 90 / *in_deg_size;
        sectsample = sectmaxsamps * counter_ratio;
        sectsample++;
        if (sectsample >= sectmaxsamps)
        {
            sectsample = 0;
            nf_sectsample = 0;
            sect++;
            switch(sect)
            {
            case 1:
                sect_startlvl = output;
                sect_spanlvl = -sect_startlvl;
                break;
            case 2:
                sect_startlvl = output;
                sect_spanlvl = sect_startlvl - 1;
                break;
            case 3:
                sect_startlvl = output;
                sect_spanlvl = -sect_startlvl;
                break;
            default:
                if (recycle == ON)
                {
                    sect = 0;
                    if (zero_retrigger_mode == ON)
                        output = 0;
                    sect_startlvl = output;
                    sect_spanlvl = 1 - sect_startlvl;
                }
                else
                    play_state = OFF;
            }
        }
    }
}

int triangle_wave2::triangle_wave2_count = 0;

#ifndef BARE_MODULES
bool triangle_wave2::done_params = false;

void triangle_wave2::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_TRIWAVE2, paramnames::PAR_NORM_FREQ);
    get_paramlist()->add_param(
     synthmodnames::MOD_TRIWAVE2, paramnames::PAR_NORM_MODSIZE);
    get_paramlist()->add_param(
     synthmodnames::MOD_TRIWAVE2, paramnames::PAR_RECYCLE_MODE);
    get_paramlist()->add_param(
     synthmodnames::MOD_TRIWAVE2, paramnames::PAR_ZERO_RETRIGGER);
    done_params = true;
}
#endif
#endif
