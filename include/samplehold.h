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
    stockerrs::ERR_TYPE validate();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, const void*);
    const void* get_param(paramnames::PAR_TYPE) const;

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
