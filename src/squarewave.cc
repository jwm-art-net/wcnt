#include "../include/squarewave.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

#include <cstdlib>

square_wave::square_wave(char const* uname) :
 synthmod(synthmodnames::SQUAREWAVE, uname, SM_HAS_OUT_OUTPUT),
 output(0.00), out_offpulse(OFF), play_state(OFF),
 in_phase_trig(NULL), in_phase_step(NULL), in_pwm(NULL),
 rate(1.0), pulse_width(0.50), pwm_size(0.00), recycle(OFF),
 pulse(OFF), degs(0.00), podeg(0), pwdeg_rad(0), poff_deg(0.00),
 target(0), cycle(0)
{
    register_output(outputnames::OUT_OUTPUT);
    register_output(outputnames::OUT_OFF_PULSE);
    register_output(outputnames::OUT_PLAY_STATE);
    register_input(inputnames::IN_PHASE_TRIG);
    register_input(inputnames::IN_PHASE_STEP);
    register_input(inputnames::IN_PWM);
    init_first();
}

square_wave::~square_wave()
{
}

void const* square_wave::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_OUTPUT:       return &output;
        case outputnames::OUT_OFF_PULSE:    return &out_offpulse;
        case outputnames::OUT_PLAY_STATE:   return &play_state;
        default: return 0;
    }
}

void const* square_wave::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_PHASE_TRIG: return in_phase_trig = (STATUS*)o;
        case inputnames::IN_PHASE_STEP: return in_phase_step = (double*)o;
        case inputnames::IN_PWM:        return in_pwm =  (double*)o;
        default: return 0;
    }
}

void const* square_wave::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_PHASE_TRIG: return in_phase_trig;
        case inputnames::IN_PHASE_STEP: return in_phase_step;
        case inputnames::IN_PWM:        return in_pwm;
        default: return 0;
    }
}

bool square_wave::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
        case paramnames::RATE:
            rate = *(double*)data;
            return true;
        case paramnames::PULSE_WIDTH:
            pulse_width = *(double*)data;
            return true;
        case paramnames::PWM_SIZE:
            pwm_size = *(double*)data;
            return true;
        case paramnames::RECYCLE_MODE:
            recycle = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

void const* square_wave::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::RATE:         return &rate;
        case paramnames::PULSE_WIDTH:  return &pulse_width;
        case paramnames::PWM_SIZE:     return &pwm_size;
        case paramnames::RECYCLE_MODE: return &recycle;
        default: return 0;
    }
}

stockerrs::ERR_TYPE square_wave::validate()
{
    if (!jwm.get_paramlist()->validate(this, paramnames::RATE,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::RATE);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::PULSE_WIDTH,
            stockerrs::ERR_RANGE_0_1_IN))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::PULSE_WIDTH);
        invalidate();
        return stockerrs::ERR_RANGE_0_1_IN;
    }
    if (!jwm.get_paramlist()->validate(this, paramnames::PWM_SIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::PWM_SIZE);
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    return stockerrs::ERR_NO_ERROR;
}

void square_wave::init()
{
    podeg = pulse_width * 360;
    if (podeg <= 180)
        pwdeg_rad = podeg * pwm_size;
    else
        pwdeg_rad = (360 - podeg) * pwm_size;
}

void square_wave::run()
{
    if (*in_phase_trig == ON)
    {
        play_state = ON;
        poff_deg = podeg + pwdeg_rad * *in_pwm;
        if (poff_deg < 1.00)
            poff_deg = 1.00;
        else if (poff_deg > 359.00)
            poff_deg = 359.00;
        target = 1;
        pulse = ON;
    }
    degs += *in_phase_step;
    if (degs >= 360.00)
        degs -= 360.00;
    if (play_state == ON)
    {
        if (degs + *in_phase_step >= 360.00)
        {
            if (recycle == OFF)
            {
                play_state = OFF;
                target = 0;
            }
            else
            {
                poff_deg = podeg + pwdeg_rad * *in_pwm;
                if (poff_deg < 1.00)
                    poff_deg = 1.00;
                else if (poff_deg > 359.00)
                    poff_deg = 359.00;
                pulse = ON;
            }
        }
        if (pulse == ON)
        {
            if (degs >= poff_deg)
            {
                target = -1.00;
                pulse = OFF;
                out_offpulse = ON;
            }
        }
        else if (out_offpulse == ON)
            out_offpulse = OFF;
    }
    output += (target - output) * rate;
}

void square_wave::init_first()
{
    if (done_first())
        return;
    register_param(paramnames::RATE);
    register_param(paramnames::PULSE_WIDTH);
    register_param(paramnames::PWM_SIZE);
    register_param(paramnames::RECYCLE_MODE);
}

