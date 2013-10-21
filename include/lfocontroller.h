#ifndef LFOCONTROLLER_H
#define LFOCONTROLLER_H

#include "synthmod.h"

class lfo_controller : public synthmod::base
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
    //--
    samp_t total_resp_samples;
    samp_t resp_samples;
    double resp_step;
    double resp_level;
    double resp_level_grab;

    samp_t total_delay_samples;
    samp_t delay_samples;

    samp_t total_ramp_samples;
    samp_t ramp_samples;
    double ramp_step;
    double ramp_level;

    double amp_center;
    double amp_half_range;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
