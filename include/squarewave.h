#ifndef SQUAREWAVE_H
#define SQUAREWAVE_H

#include "synthmodule.h"
#include "modoutputslist.h"
#include "modinputslist.h"
#include "modparamlist.h"

class square_wave : public synthmod
{
public:
    square_wave(char const*);
    ~square_wave();
    void set_input_phase_trig(const STATUS* t){ in_phase_trig = t;}
    void set_input_deg_size(const double* ds){ in_deg_size = ds;}
    void set_input_pwm(const double* pwm){ in_pwm = pwm;}
    const STATUS* get_input_phase_trig(){ return in_phase_trig;}
    const double* get_input_deg_size(){ return in_deg_size;}
    const double* get_input_pwm(){ return in_pwm;}
    const double* get_output(){ return &output;}
    const STATUS* get_output_off_pulse(){ return &out_offpulse;}
    const STATUS* get_play_state(){ return &play_state;}
    void set_pulse_width(double pw){ pulse_width = pw;}
    void set_pwm_size(double ps){ pwm_size = ps;}
    void set_recycle_mode(STATUS rm){ recycle = rm;}
    double get_pulse_width(){ return pulse_width;}
    double get_pwm_size(){ return pwm_size;}
    STATUS get_recycle_mode(){ return recycle;}
    // virtual funcs
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE);
    void const* set_in(inputnames::IN_TYPE, void const*);
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE);

private:
    double output;
    STATUS out_offpulse;
    STATUS play_state;
    const STATUS* in_phase_trig;
    const double* in_deg_size;
    const double* in_pwm;
    double degs;
    double podeg;
    double pwdeg_rad;
    double pulse_width;
    double pwm_size;
    double poff_deg;
    STATUS pulse;
    STATUS recycle;
    short cycle;
    static int square_wave_count;
    void create_params();
    static bool done_params;
};

#endif
