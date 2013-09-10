#ifndef FREQGENERATOR_H
#define FREQGENERATOR_H

#include "synthmodule.h"

class freq_generator : public synthmod
{
public:
    freq_generator(char const*);
    ~freq_generator();
    void run();
    void init();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

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
    void create_params();
    static bool done_params;
};

#endif
