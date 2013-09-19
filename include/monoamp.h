#ifndef MONOAMP_H
#define MONOAMP_H

#include "synthmod.h"

// because we're now using libsndfile to handle file input/output
// we no longer have 16bit integer input/output and amplitude levels

class mono_amp : public synthmod
{
 public:
    mono_amp(char const*);
    ~mono_amp();
    void run();
    stockerrs::ERR_TYPE validate();
    void const* get_out(outputnames::OUT_TYPE) const;
    void const* set_in(inputnames::IN_TYPE, void const*);
    const void* get_in(inputnames::IN_TYPE it) const;
    bool set_param(paramnames::PAR_TYPE, void const*);
    void const* get_param(paramnames::PAR_TYPE) const;

 private:
    // inputs
    const double* in_signal;
    const double* in_amp_eg;
    const double* in_amp_mod;

    // outputs
    double out_output;

    // params
    double level;
    double amp_modsize;
    double clip_level;

    // working
    double amp_level;
    double ampsig;
    void init_first();
    
};

#endif
