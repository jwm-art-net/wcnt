#ifndef SAMPLEHOLD_H
#define SAMPLEHOLD_H

#include "synthmod.h"

/* decay time of 0 means do not decay to zero */

class sample_hold : public synthmod
{
public:
    sample_hold(const char*);
    ~sample_hold();
    void run();
    void init();
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

private:
    const STATUS* in_trig;
    const double* in_signal;
    double output;
    double decay_time;
    samp_t decay_samps;
    samp_t ds;
    double decay_size;
    void init_first();
};

#endif
