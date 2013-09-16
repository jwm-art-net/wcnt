#ifndef SQUAREWAVE_H
#define SQUAREWAVE_H

#include "synthmod.h"

class square_wave : public synthmod
{
public:
    square_wave(char const*);
    ~square_wave();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

private:
    double output;
    STATUS out_offpulse;
    STATUS play_state;
    const STATUS* in_phase_trig;
    const double* in_phase_step;
    const double* in_pwm;
    double rate;
    double pulse_width;
    double pwm_size;
    STATUS recycle;
    STATUS pulse;
    double degs;
    double podeg;
    double pwdeg_rad;
    double poff_deg;
    double target;
    short cycle;
    void create_params();
    
};

#endif
