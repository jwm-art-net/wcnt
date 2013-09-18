#ifndef LFOCONTROLLER_H
#define LFOCONTROLLER_H

#include "synthmod.h"

class lfo_controller : public synthmod
{
public:
    lfo_controller(char const*);
    ~lfo_controller();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

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
    unsigned long delay_samples;
    unsigned long ramp_samples;
    double resp_size;
    double resp_fa_level;
    double resp_ac;
    double level_size;
    double current_level;
    void init_first();
    
};

#endif
