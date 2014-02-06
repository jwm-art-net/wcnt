#ifndef SAMPLEHOLD_H
#define SAMPLEHOLD_H

#include "synthmod.h"

/* decay time of 0 means do not decay to zero */

class sample_hold : public synthmod::base
{
public:
    sample_hold(const char*);
    ~sample_hold();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(int output_type) const;
    const void* set_in(int input_type, const void*);
    const void* get_in(int input_type) const;
    bool set_param(int param_type, const void*);
    const void* get_param(int param_type) const;

private:
    const STATUS* in_trig;
    const double* in_signal;
    double output;
    double decay_time;
    samp_t decay_samps;
    samp_t ds;
    double decay_size;
    void register_ui();
    ui::moditem_list* get_ui_items();
};

#endif
