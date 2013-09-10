#ifndef TRIANGLEWAVE2_H
#include "../include/trianglewave2.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/conversions.h"

triangle_wave2::triangle_wave2(char const* uname) :
 synthmod(synthmodnames::TRIWAVE2, uname),
 in_phase_trig(0), in_phase_step(0), in_normal_mod(0), output(0.00),
 play_state(OFF), normal_freq(440.00), norm_modsize(0), recycle(OFF),
 zero_retrigger_mode(OFF), nf_phase_step(0), nf_pre_phase_step(0),
 nf_sectmaxsamps(0), nf_sectsample(0), nf_counter_ratio(0.0), sect(0),
 sect_spanlvl(0), sect_startlvl(0), old_maxsamps(0), sectmaxsamps(1),
 sectsample(0), counter_ratio(0)
{
    jwm.get_outputlist()->add_output(this, outputnames::OUT_OUTPUT);
    jwm.get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE);
    jwm.get_inputlist()->add_input(this, inputnames::IN_PHASE_TRIG);
    jwm.get_inputlist()->add_input(this, inputnames::IN_PHASE_STEP);
    jwm.get_inputlist()->add_input(this, inputnames::IN_NORM_MOD);
    create_params();
}

triangle_wave2::~triangle_wave2()
{
}

void const* triangle_wave2::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT:       return &output;
        case outputnames::OUT_PLAY_STATE:   return &play_state;
        default: return 0;
    }
}

void const* triangle_wave2::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_PHASE_TRIG: return in_phase_trig = (STATUS*)o;
        case inputnames::IN_PHASE_STEP: return in_phase_step = (double*)o;
        case inputnames::IN_NORM_MOD:   return in_normal_mod = (double*)o;
        default: return 0;
    }
}

void const* triangle_wave2::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_PHASE_TRIG: return in_phase_trig;
        case inputnames::IN_PHASE_STEP: return in_phase_step;
        case inputnames::IN_NORM_MOD:   return in_normal_mod;
        default: return 0;
    }
}

bool triangle_wave2::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::NORM_FREQ:
            normal_freq = *(double*)data;
            return true;
        case paramnames::NORM_MODSIZE:
            norm_modsize = *(double*)data;
            return true;
        case paramnames::RECYCLE_MODE:
            recycle = *(STATUS*)data;
            return true;
        case paramnames::ZERO_RETRIGGER:
            zero_retrigger_mode = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

void const* triangle_wave2::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::NORM_FREQ:     return &normal_freq;
        case paramnames::NORM_MODSIZE:  return &norm_modsize;
        case paramnames::RECYCLE_MODE:  return &recycle;
        case paramnames::ZERO_RETRIGGER:return &zero_retrigger_mode;
        default: return 0;
    }
}

stockerrs::ERR_TYPE triangle_wave2::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::NORM_FREQ,
            stockerrs::ERR_RANGE_FREQ))
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::NORM_FREQ);
        invalidate();
        return stockerrs::ERR_RANGE_FREQ;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::NORM_MODSIZE,
            stockerrs::ERR_RANGE_FMOD))
    {
        *err_msg
         = jwm.get_paramnames()->get_name(paramnames::NORM_MODSIZE);
        invalidate();
        return stockerrs::ERR_RANGE_FMOD;
    }
    return stockerrs::ERR_NO_ERROR;
}

void triangle_wave2::init()
{
    nf_pre_phase_step = freq_to_step(normal_freq, 0);
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
            nf_phase_step = nf_pre_phase_step
             / (1 + norm_modsize * -*in_normal_mod);
        else
            nf_phase_step = nf_pre_phase_step
             * (1 + norm_modsize * *in_normal_mod);
        nf_counter_ratio = nf_sectsample / nf_sectmaxsamps;
        nf_sectmaxsamps = 90 / nf_phase_step;
        nf_sectsample = nf_sectmaxsamps * nf_counter_ratio;
        nf_sectsample++;
        if (nf_sectsample < nf_sectmaxsamps)
            output = sect_startlvl + sect_spanlvl * nf_counter_ratio;
        counter_ratio = sectsample / sectmaxsamps;
        sectmaxsamps = 90 / *in_phase_step;
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

bool triangle_wave2::done_params = false;

void triangle_wave2::create_params()
{
    if (done_params == true)
        return;
    jwm.get_paramlist()->add_param(
        synthmodnames::TRIWAVE2, paramnames::NORM_FREQ);
    jwm.get_paramlist()->add_param(
     	synthmodnames::TRIWAVE2, paramnames::NORM_MODSIZE);
    jwm.get_paramlist()->add_param(
     	synthmodnames::TRIWAVE2, paramnames::RECYCLE_MODE);
    jwm.get_paramlist()->add_param(
        synthmodnames::TRIWAVE2, paramnames::ZERO_RETRIGGER);
    done_params = true;
}

#endif
