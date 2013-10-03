#ifndef LFOCONTROLLER_H
#define LFOCONTROLLER_H

#include "synthmod.h"

class lfo_controller : public synthmod
{
public:
    lfo_controller(const char*);
    ~lfo_controller();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    // inputs
    const STATUS* in_trig;
    const double* in_wave;
    const double* in_amp_mod;
    // outputs
    double out_preampmod;
    double output;
    // params
    double delay_time;
    double ramp_time;
    double start_level;
    double end_level;
    double response_time;
    double amp_modsize;
    // working
    double ams_r;
    samp_t delay_samples;
    samp_t ramp_samples;
    double resp_size;
    double resp_fa_level;
    double resp_ac;
    double level_size;
    double current_level;
    void init_first();
};

#endif
