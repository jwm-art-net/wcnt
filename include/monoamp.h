#ifndef MONOAMP_H
#define MONOAMP_H

#include "gain.h"

// because we're now using libsndfile to handle file input/output
// we no longer have 16bit integer input/output and amplitude levels

class mono_amp : public synthmod, public gain
{
 public:
    mono_amp(const char*);
    ~mono_amp();
    void run();
    void init() { gain::init(); }
    errors::TYPE validate();
    const void* get_out(output::TYPE) const;
    const void* set_in(input::TYPE, const void*);
    const void* get_in(input::TYPE) const;
    bool set_param(param::TYPE, const void*);
    const void* get_param(param::TYPE) const;

 private:
    // inputs
    const double* in_signal;
    const double* in_amp_eg;

    // outputs
    double out_output;

    // params
    double clip_level;

    // working
    void init_first();
};

#endif
