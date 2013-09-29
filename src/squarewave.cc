#include "../include/squarewave.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"

#include <cstdlib>

square_wave::square_wave(const char* uname) :
 synthmod(module::SQUAREWAVE, uname, SM_HAS_OUT_OUTPUT),
 output(0.00), out_offpulse(OFF), play_state(OFF),
 in_phase_trig(NULL), in_phase_step(NULL), in_pwm(NULL),
 rate(1.0), pulse_width(0.50), pwm_size(0.00), recycle(OFF),
 pulse(OFF), degs(0.00), podeg(0), pwdeg_rad(0), poff_deg(0.00),
 target(0), cycle(0)
{
    register_input(input::IN_PHASE_TRIG);
    register_input(input::IN_PHASE_STEP);
    register_input(input::IN_PWM);
    register_output(output::OUT_OUTPUT);
    register_output(output::OUT_OFF_PULSE);
    register_output(output::OUT_PLAY_STATE);
    init_first();
}

square_wave::~square_wave()
{
}

const void* square_wave::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_OUTPUT:       return &output;
        case output::OUT_OFF_PULSE:    return &out_offpulse;
        case output::OUT_PLAY_STATE:   return &play_state;
        default: return 0;
    }
}

const void* square_wave::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_PHASE_TRIG: return in_phase_trig = (STATUS*)o;
        case input::IN_PHASE_STEP: return in_phase_step = (double*)o;
        case input::IN_PWM:        return in_pwm =  (double*)o;
        default: return 0;
    }
}

const void* square_wave::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_PHASE_TRIG: return in_phase_trig;
        case input::IN_PHASE_STEP: return in_phase_step;
        case input::IN_PWM:        return in_pwm;
        default: return 0;
    }
}

bool square_wave::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::RATE:
            rate = *(double*)data;
            return true;
        case param::PULSE_WIDTH:
            pulse_width = *(double*)data;
            return true;
        case param::PWM_SIZE:
            pwm_size = *(double*)data;
            return true;
        case param::RECYCLE_MODE:
            recycle = *(STATUS*)data;
            return true;
        default:
            return false;
    }
}

const void* square_wave::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::RATE:         return &rate;
        case param::PULSE_WIDTH:  return &pulse_width;
        case param::PWM_SIZE:     return &pwm_size;
        case param::RECYCLE_MODE: return &recycle;
        default: return 0;
    }
}

stockerrs::ERR_TYPE square_wave::validate()
{
    if (!jwm.get_paramlist()->validate(this, param::RATE,
            stockerrs::ERR_RANGE_0_1))
    {
        sm_err("%s", param::names::get(param::RATE));
        invalidate();
        return stockerrs::ERR_RANGE_0_1;
    }
    if (!jwm.get_paramlist()->validate(this, param::PULSE_WIDTH,
            stockerrs::ERR_RANGE_0_1_IN))
    {
        sm_err("%s", 
                param::names::get(param::PULSE_WIDTH));
        invalidate();
        return stockerrs::ERR_RANGE_0_1_IN;
    }
    if (!jwm.get_paramlist()->validate(this, param::PWM_SIZE,
            stockerrs::ERR_RANGE_0_1))
    {
        sm_err("%s", param::names::get(param::PWM_SIZE));
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
    register_param(param::RATE);
    register_param(param::PULSE_WIDTH);
    register_param(param::PWM_SIZE);
    register_param(param::RECYCLE_MODE);
}

