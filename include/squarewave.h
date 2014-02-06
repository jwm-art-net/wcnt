#ifndef SQUAREWAVE_H
#define SQUAREWAVE_H

#include "synthmod.h"

class square_wave : public synthmod::base
{
public:
    square_wave(const char*);
    ~square_wave();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

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
    wcint_t cycle;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
