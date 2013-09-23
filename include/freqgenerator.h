#ifndef FREQGENERATOR_H
#define FREQGENERATOR_H

#include "synthmod.h"

class freq_generator : public synthmod
{
public:
    freq_generator(const char*);
    ~freq_generator();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    const void* get_out(outputnames::OUT_TYPE) const;
    const void* set_in(inputnames::IN_TYPE, const void*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, const void*);
    const void* get_param(paramnames::PAR_TYPE) const;

private:
    const double* in_signal;
    double	out_freq,
    out_phase_step,
    sig_range_hi,
    sig_range_lo,
    freq_range_hi,
    freq_range_lo,
    sig_step_size,
    freq_step_size;
    short step_count;
    void init_first();
    
};

#endif
